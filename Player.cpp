#include "Player.h"
#include "Engine/Direct3D.h"
#include "Engine/Input.h"
#include "Engine/SphereCollider.h"
#include "Ground.h"
#include "Engine/Camera.h"
#include "RenderTexture.h"
#include <iostream>
#include <algorithm>
#include "Engine/Image.h"
#include "Engine/Audio.h"
#include "FPSGun.h"
#include "Controller.h"

namespace
{
    const XMFLOAT3 PLAYER_START_POS = { 70.0f, 1.0f, -70.0f };//プレイヤーの初期位置
    constexpr float MOVE_SPEED = 0.5f;//プレイヤーの移動速度
    constexpr float PLAYER_START_ROTATE = 90.0f;//プレイヤーのスタート時の回転
    constexpr float PLAYER_COLIDER_SIZE= 1.5f;//プレイヤーのコライダーのサイズ
    constexpr float CAMERA_SENSITIVITY = 1.5f;//プレイヤーのカメラ感度
    constexpr float RAYCAST_DIST = 30.0f;//レイキャストの最大距離
}



//コンストラクタ
Player::Player(GameObject* parent)
    :GameObject(parent, "Player"), moveSpeed_(0),hCrossHair_(-1),cameraSensitivity_(0),centerX_(0),centerY_(0)
    ,isPlay_(false),fpsCamera(nullptr),fpsGun(nullptr),controller(nullptr)
{

}

//デストラクタ
Player::~Player()
{
    ShowCursor(TRUE);
}

//初期化
void Player::Initialize()
{
    
    isPlay_ = true;

    hCrossHair_ = Image::Load("crosshair.png");

    //画面中央の座標を取得
    centerX_ = Direct3D::screenWidth_ / 2;
    centerY_ = Direct3D::screenHeight_ / 2;

    //各種初期化
	transform_.position_ = PLAYER_START_POS;
    moveSpeed_ = MOVE_SPEED;
	cameraSensitivity_ = CAMERA_SENSITIVITY;
    transform_.rotate_ = { 0,PLAYER_START_ROTATE,0 };
    
    //FPS視点用
    Instantiate<FPSCamera>(this);
    Instantiate<FPSgun>(this);
    Instantiate<Controller>(this);

    //コライダーの追加
    SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), PLAYER_COLIDER_SIZE);
    AddCollider(collision);

    //カメラ
    fpsCamera = (FPSCamera*)FindChildObject("FPSCamera");

    //銃
    fpsGun = (FPSgun*)FindChildObject("FPSgun");

    //コントローラー
    controller = (Controller*)FindObject("Controller");
}

//更新
void Player::Update()
{
    if (isPlay_) {
        //プレイヤーの移動
        {
            XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
            XMMATRIX mRotate = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
            XMVECTOR vMoveForward = { 0,0,1,0 };
            XMVECTOR vMoveRight = { 1,0,0,0 };

            vMoveForward = XMVector3TransformNormal(vMoveForward, mRotate);
            vMoveRight = XMVector3TransformNormal(vMoveRight, mRotate);

            XMFLOAT2 move = controller->GetMoveInput();

            vPos += vMoveForward * move.y * moveSpeed_;
            vPos += vMoveRight * move.x * moveSpeed_;
            XMStoreFloat3(&transform_.position_, vPos);
        }

        //レイキャストを飛ばす方向の計算
        XMVECTOR ganTarget;
        XMFLOAT3 camPos = Camera::GetPosition();
        XMFLOAT3 camTar = Camera::GetTarget();
        XMVECTOR vCamPosition = XMLoadFloat3(&camPos);
        XMVECTOR vCamTarget = XMLoadFloat3(&camTar);
        ganTarget = vCamTarget - vCamPosition;

        RayCastData gan;
        gan.start = Camera::GetPosition();
        XMStoreFloat3(&gan.dir, ganTarget);


        //マウスのボタンが押されたら
        if (Input::IsMouseButton(0))
        {
            fpsGun->BangEffect();//エフェクトを出す
            this->RayCastToPaintObjects(gan);
            
        }

        this->OnGround();

        fpsCamera->SetFpsCamera(transform_, cameraSensitivity_);//カメラの更新
    }
}

//描画
void Player::Draw()
{
    //クロスヘアの表示
    Transform ch;
    ch.matTranslate_ = XMMatrixTranslation(centerX_, centerY_, 0);
    ch.scale_ = { 1.0,1.0,0 };
    Image::SetTransform(hCrossHair_, ch);
    Image::Draw(hCrossHair_);
}

//開放
void Player::Release()
{

}

void Player::OnGround()
{
    Ground* pGround = (Ground*)FindObject("Ground");    //ステージオブジェクトを探す
    int hGroundModel = pGround->GetModelHandle();    //モデル番号を取得

    RayCastData data;
    data.start = transform_.position_;   //レイの発射位置
    data.start.y = 0;

    data.dir = XMFLOAT3(0, -1, 0);       //レイの方向
    Model::RayCast(hGroundModel, &data); //レイを発射

    //レイが当たったら
    if (data.hit)
    {
        //その分位置を下げる
        transform_.position_.y = -data.dist;//当たった距離のマイナス、ステージの最高点が0より低い
    }
}

bool Player::RayCastToPaintObjects(RayCastData& data)
{
    PaintObject* closestObj = nullptr;
    float dist = RAYCAST_DIST;
    XMFLOAT2 UV = { 0,0 };
	XMFLOAT3 normal = { 0,0,0 };
	XMFLOAT3 hitPos = { 0,0,0 };
    // PaintObject 継承クラスだけに絞ってループ
    for (PaintObject* pObj : PaintObject::GetPaintObjectList()) {
        RayCastData ray = data; // コピーして使用
        // 描画されていない、または死んでいるオブジェクトはスキップ
        if (!pObj->IsVisibled() || pObj->IsDead()) continue;


        // GameObject の hModel_ 使用して判定 
        Model::RayCast(pObj->GetModelHandle(), &ray);

        //レイがヒットしたら
        if (ray.hit)
        {
            if (dist > ray.dist)
            {
                dist = ray.dist;//距離を入れる
                closestObj = pObj;
                UV = ray.uv;//uvを入手
                normal = ray.normal;//法線を入手
                hitPos = XMFLOAT3(//hitposの計算
                    data.start.x + data.dir.x * ray.dist,
                    data.start.y + data.dir.y * ray.dist,
                    data.start.z + data.dir.z * ray.dist
                );

            }

        }
    }
    //ポインタがnullじゃなくなったら
    if (closestObj != nullptr)
    {
        //オブジェクトにモザイクを塗る
       closestObj->PaintMosaic(UV,hitPos,normal);
	   return true;
    }
	return false;
}

void Player::OnCollision(GameObject* pTarget,HitResult result)
{
    //ビルと当たったら跳ね返す
    if (pTarget->GetObjectName() == "Building")
    {
		XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
		XMVECTOR vPushBack = XMLoadFloat3(&result.pushBack);
        vPos = vPos + vPushBack;
        XMStoreFloat3(&transform_.position_, vPos);
    }
}
