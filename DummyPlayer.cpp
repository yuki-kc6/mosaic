#include "DummyPlayer.h"
#include "Player.h"
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
	hClearModel_ = Model::Load("Models/Player_ClearDance.fbx");
	transform_.position_.y = 1;
	transform_.position_.z = -70;
	transform_.position_.x = 70;
	transform_.scale_ = { 0.05,0.05,0.05 };
	transform_.rotate_ = { 0,90,0 };
	state_ = IDLE;
	Model::SetAnimFrame(hModel_, 0.0f, 60.0f, 1.0f);
	Model::SetAnimFrame(hClearModel_, 0.0f, 30.0f, 0.75f);
	if (GetParent()->GetObjectName() == "PlayScene") {
		Invisible();

	}

}

void DummyPlayer::Update()
{
	if (GetParent()->GetObjectName() == "PlayScene") {
		transform_.position_=FindObject("Player")->GetPosition();
	}

	
}

void DummyPlayer::Draw()
{
	if (state_ == IDLE) {
		Model::SetTransform(hModel_, transform_);
		Model::Draw(hModel_);
	}
	else if (state_ == CLEAR) {
		Model::SetTransform(hClearModel_, transform_);
		Model::Draw(hClearModel_);
	}
}

void DummyPlayer::Release()
{
}
void DummyPlayer::SetState(PlayerState state)
{
	 state_ = state; 
	 
}
