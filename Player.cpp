#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"
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

    if (Input::IsKey(DIK_W))
    {
        transform_.position_.z += 0.1;
    }
    if (Input::IsKey(DIK_S))
    {
        transform_.position_.z -= 0.1;
    }
    if (Input::IsKey(DIK_D))
    {
        transform_.position_.x+= 0.1;
    }
    if (Input::IsKey(DIK_A))
    {
        transform_.position_.x -= 0.1;
    }
  
    if (Input::IsKey(DIK_RIGHT))
    {
        transform_.rotate_.y += 1.0;
    }
    if (Input::IsKey(DIK_LEFT))
    {
        transform_.rotate_.y -= 1.0;
    }

    //三人称
   /*XMVECTOR vCam = { 0, 5.0f, -10.0f, 0 };
   vCam = XMVector3TransformCoord(vCam, mRotate);
   XMFLOAT3 camPos;
   XMStoreFloat3(&camPos, vPos + vCam);
   Camera::SetPosition(camPos);

   XMFLOAT3 camTarget = transform_.position_;

   Camera::SetTarget(camTarget);
*/

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