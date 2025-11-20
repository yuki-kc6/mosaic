#include "Bullet.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"

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

    SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), 1.2f);
    AddCollider(collision);
    XMMATRIX mRotate = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
   
}

void Bullet::Update()
{
    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
    XMVECTOR vMove1 = { 0,0,5.0f,0 };
   

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

void Bullet::OnCollision(GameObject* pTarget)
{
    pTarget->PaintMosaic();
}

void Bullet::SetDir(XMMATRIX rotate)
{
    mRotate = rotate;
}

