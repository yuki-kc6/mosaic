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
    hModel_ = Model::Load("Models/enemy2.fbx");
    assert(hModel_ >= 0);

    transform_.position_.y = 1;

    transform_.scale_ = { 5.0,5.0,5.0 };

}

void Enemy::Update()
{
    //ビルから出てきて、ランダムな他のビルに向かって移動するというのをやりたい
	transform_.position_.x += 0.1f;
}

void Enemy::Draw()
{
    Model::SetTransform(hModel_, transform_,GetMosaicRT()->GetShaderResourceView());
    Model::Draw(hModel_);
}

void Enemy::Release()
{
}

