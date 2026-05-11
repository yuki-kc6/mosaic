#include "Player.h"
#include "Engine/Direct3D.h"
#include "Engine/Input.h"
#include "Engine/SphereCollider.h"
#include "Ground.h"
#include "Engine/Camera.h"
#include "Bullet.h"
#include "RenderTexture.h"
#include <iostream>
#include <algorithm>
#include "Engine/Image.h"
#include "Engine/Audio.h"


//コンストラクタ
Player::Player(GameObject* parent)
    :GameObject(parent, "Player"), hModel_(-1),moveSpeed_(0)
{

}

//デストラクタ
Player::~Player()
{
    //Image::Release(hCrossHair_);
    ShowCursor(TRUE);
}

//初期化
void Player::Initialize()
{
    //hModel_ = Model::Load("Models/PlayerKari.fbx");
    //assert(hModel_ >= 0);

    hCrossHair_ = Image::Load("crosshair.png");
    centerX = Direct3D::screenWidth_ / 2;
    centerY = Direct3D::screenHeight_ / 2;


    transform_.position_.y = 1;
    moveSpeed_ = 0.08;

    transform_.rotate_ = { 0,0,0 };
    
    Instantiate<FPSCamera>(this);
    
    SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), 1.2f);
    AddCollider(collision);



}

//更新
void Player::Update()
{
    //カメラ
	fpsCamera = (FPSCamera*)FindChildObject("FPSCamera");
    fpsCamera->SetFpsCamera(transform_,0.5f);

 

    //プレイヤーの移動
    {
      XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
      XMMATRIX mRotate = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
      XMVECTOR vMoveForward = { 0,0,1,0 };
      XMVECTOR vMoveRight = { 1,0,0,0 };

      vMoveForward = XMVector3TransformNormal(vMoveForward, mRotate);
      vMoveRight = XMVector3TransformNormal(vMoveRight, mRotate);

     //移動
        if (Input::IsKey(DIK_W))
        {
            vPos += vMoveForward*moveSpeed_;
        }
        if (Input::IsKey(DIK_S))
        {
            vPos -= vMoveForward*moveSpeed_;
        }
        if (Input::IsKey(DIK_D))
        {
            vPos += vMoveRight*moveSpeed_;
        }
        if (Input::IsKey(DIK_A))
        {
            vPos -= vMoveRight*moveSpeed_;
        }
        XMStoreFloat3(&transform_.position_, vPos);
    }


    if (Input::IsKey(DIK_SPACE))
    {
        transform_.position_.y += 0.3;
    }
    if (Input::IsKey(DIK_B))
    {
        transform_.position_.y -= 0.3;
    }


    XMVECTOR ganTarget;
    XMFLOAT3 camPos = Camera::GetPosition();
    XMFLOAT3 camTar = Camera::GetTarget();
    XMVECTOR vCamPosition = XMLoadFloat3(&camPos);
    XMVECTOR vCamTarget= XMLoadFloat3(&camTar);
    ganTarget = vCamTarget-vCamPosition;

    RayCastData gan;
    gan.start = Camera::GetPosition();
    XMStoreFloat3(&gan.dir,ganTarget);
   

    if (Input::IsMouseButton(0))
    {
        this->RayCastToPaintObjects(gan);
    }

    //this->OnGround();
}

//描画
void Player::Draw()
{
    Transform ch;
    ch.matTranslate_ = XMMatrixTranslation(centerX, centerY, 0);
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

void Player::RayCastToPaintObjects(RayCastData& data)
{
    PaintObject* closestObj = nullptr;
    float dist = FLT_MAX;
    XMFLOAT2 UV = { 0,0 };
    // PaintObject 継承クラスだけに絞ってループ
    for (PaintObject* pObj : PaintObject::GetPaintObjectList()) {
        RayCastData ray = data; // コピーして使用
        // 描画されていない、または死んでいるオブジェクトはスキップ
        if (!pObj->IsVisibled() || pObj->IsDead()) continue;
        // GameObject の hModel_ 使用して判定 
        Model::RayCast(pObj->GetModelHandle(), &ray);

        if (ray.hit)
        {
            if (dist > ray.dist)
            {
                dist = ray.dist;
                closestObj = pObj;
                UV = ray.uv;
            }
        }
    }
    if (closestObj != nullptr)
    {
        //オブジェクトにモザイクを塗る
       closestObj->PaintMosaic(UV);
    }
}

void Player::OnCollision(GameObject* pTarget,HitResult result)
{
    if (pTarget->GetObjectName() == "Building")
    {
		XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
		XMVECTOR vPushBack = XMLoadFloat3(&result.pushBack);
        vPos = vPos - vPushBack;
        XMStoreFloat3(&transform_.position_, vPos);
    }
}
