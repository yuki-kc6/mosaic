#include "Bullet.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"

Bullet::Bullet(GameObject* parent)
	:GameObject(parent, "Bullet"), hModel_(-1),life_(100)
{

}

Bullet::~Bullet()
{

}

void Bullet::Initialize()
{
    hModel_ = Model::Load("Models/BulletKari.fbx");
    assert(hModel_ >= 0);

    SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), 1.2f);
    AddCollider(collision);
   
    move_ = XMVectorSet(0, 0.1f, 0, 0);
}

void Bullet::Update()
{
    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
 
 

    vPos += move_;
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



