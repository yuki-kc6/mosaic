#include "Player.h"
#include "Engine/Input.h"
#include "Ground.h"
#include "Engine/Camera.h"
#include "Bullet.h"
#include "RenderTexture.h"
#include <iostream>
#include <algorithm>

//コンストラクタ
Player::Player(GameObject* parent)
    :GameObject(parent, "Player"), hModel_(-1),moveSpeed_(0)
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
    //hModel_ = Model::Load("Models/PlayerKari.fbx");
    //assert(hModel_ >= 0);

    transform_.position_.y = 1;
    moveSpeed_ = 0.08;

    transform_.rotate_ = { 0,0,0 };
    
    fpsCamera = new FPSCamera();
    
    //ShowCursor(FALSE);
}

//更新
void Player::Update()
{
    //カメラ
    fpsCamera->Update();
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
    //Model::SetTransform(hModel_, transform_);
    //Model::Draw(hModel_);

}

//開放
void Player::Release()
{
    delete fpsCamera;
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
        // 描画されていない、または死んでいるオブジェクトはスキップ
        RayCastData ray = data; // コピーして使用
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
       closestObj->PaintMosaic(UV,0.05);
    }
}

void Player::PlayerCamera()
{

}
