#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"
#include "Bullet.h"
//コンストラクタ
Player::Player(GameObject* parent)
    :GameObject(parent, "Player"), hModel_(-1)
{
}

//デストラクタ
Player::~Player()
{
}

//初期化
void Player::Initialize()
{
    hModel_ = Model::Load("Models/PlayerKari.fbx");
    assert(hModel_ >= 0);
  
    transform_.position_.y = 1;
}

//更新
void Player::Update()
{
    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
    XMVECTOR vMove1 = { 0,0,0.1f,0 };
    XMVECTOR vMove2 = { 0.1f,0,0,0 };

    XMMATRIX mRotate = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));

    vMove1 = XMVector3TransformCoord(vMove1, mRotate);
    vMove2 = XMVector3TransformCoord(vMove2, mRotate);

    if (Input::IsKey(DIK_W))
    {
        vPos += vMove1;
        XMStoreFloat3(&transform_.position_, vPos);
    }
    if (Input::IsKey(DIK_S))
    {
        vPos -= vMove1;
        XMStoreFloat3(&transform_.position_, vPos);
    }
    if (Input::IsKey(DIK_D))
    {
        vPos += vMove2;
        XMStoreFloat3(&transform_.position_, vPos);
    }
    if (Input::IsKey(DIK_A))
    {
        vPos -= vMove2;
        XMStoreFloat3(&transform_.position_, vPos);
    }
  
    if (Input::IsKey(DIK_RIGHT))
    {
        transform_.rotate_.y += 1.0;
    }
    if (Input::IsKey(DIK_LEFT))
    {
        transform_.rotate_.y -= 1.0;
    }

    XMStoreFloat3(&transform_.position_, vPos);

    if (Input::IsKey(DIK_RETURN))
    {
        GameObject* pBullet=Instantiate<Bullet>(GetParent());
        pBullet->SetPosition(transform_.position_);
    }


   //三人称
   XMVECTOR vCam = { 0, 10.0f, -10.0f, 0 };
   vCam = XMVector3TransformCoord(vCam, mRotate);
   XMFLOAT3 camPos;
    XMStoreFloat3(&camPos, vPos + vCam);
   Camera::SetPosition(camPos);//カメラの場所

   XMFLOAT3 camTarget = transform_.position_;

   Camera::SetTarget(camTarget);//カメラのターゲット

}

//描画
void Player::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
    
}

//開放
void Player::Release()
{
}