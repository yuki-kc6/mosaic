#include "Enemy.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"

Enemy::Enemy(GameObject* parent)
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
}

void Enemy::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void Enemy::Release()
{
}

void Enemy::OnCollision(GameObject* pTarget)
{
    if (pTarget->GetObjectName() == "Bullet")
    {
        KillMe();
    }
}
