#include "Enemy.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"

Enemy::Enemy(GameObject* parent)
    : PaintObject(parent, "Enemy")
{

}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
    hModel_ = Model::Load("Models/Enemy2.fbx");
    assert(hModel_ >= 0);

    transform_.position_.y = 1;

    SphereCollider* collision = new SphereCollider(transform_.position_, 1.2f);
    AddCollider(collision);



}

void Enemy::Update()
{
	transform_.position_.x += 0.01f;

}

void Enemy::Draw()
{
    Model::SetTransform(hModel_, transform_,GetMosaicRT()->GetShaderResourceView());
    Model::Draw(hModel_);
}

void Enemy::Release()
{
}

void Enemy::OnCollision(GameObject* pTarget)
{
   
}
