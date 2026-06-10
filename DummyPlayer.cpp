#include "DummyPlayer.h"
#include "Engine/Model.h"

DummyPlayer::DummyPlayer(GameObject* parent)
	:GameObject(parent,"DummyPlayer")
{
}

DummyPlayer::~DummyPlayer()
{
}

void DummyPlayer::Initialize()
{
	hModel_ = Model::Load("Models/Player.fbx");
	transform_.position_.y = 1;
	transform_.position_.z = -70;
	transform_.position_.x = 70;
	transform_.scale_ = { 0.1,0.1,0.1 };
	transform_.rotate_ = { 0,90,0 };
}

void DummyPlayer::Update()
{
}

void DummyPlayer::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void DummyPlayer::Release()
{
}
