#include "Bullet.h"
#include "Engine/Model.h"

Bullet::Bullet(GameObject* parent)
	:GameObject(parent, "Bullet"), hModel_(-1)
{

}

Bullet::~Bullet()
{
}

void Bullet::Initialize()
{
    hModel_ = Model::Load("Models/BulletKari.fbx");
    assert(hModel_ >= 0);

    life_= 100;

}

void Bullet::Update()
{
    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
    XMVECTOR vMove1 = { 0,0,0.1f,0 };
    XMMATRIX mRotate = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));

    vMove1 = XMVector3TransformCoord(vMove1, mRotate);

    vPos += vMove1;
    XMStoreFloat3(&transform_.position_, vPos);


    life_--;//ñàÉtÉåÅ[ÉÄ1Ç∏Ç¬å∏ÇÈ
    if (life_ <= 0)
    {
        KillMe();//éÄ
    }

}

void Bullet::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);

}

void Bullet::Release()
{
}
