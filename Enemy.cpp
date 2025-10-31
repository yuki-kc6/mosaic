#include "Enemy.h"
#include "Engine/Model.h"

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
