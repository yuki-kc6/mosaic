#include "DummyPlayer.h"
#include "Player.h"
#include "Engine/Model.h"

namespace
{
	constexpr float DUMMY_PLAYER_SCALE = 0.05f;
	constexpr float DUMMY_PLAYER_Y = 1.0f;
	constexpr float DUMMY_PLAYER_Z = -70.0f;
	constexpr float DUMMY_PLAYER_X = 70.0f;
	constexpr float DUMMY_PLAYER_ROTATE = 60.0f;

	constexpr float ANIM_START_FRAME = 0.0f;

	constexpr float WALK_ANIM_END = 60.0f;
	constexpr float WALK_ANIM_SPEED = 1.0f;

	constexpr float CLEAR_ANIM_END = 30.0f;
	constexpr float CLEAR_ANIM_SPEED = 0.75f;
}

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
	transform_.position_.y = DUMMY_PLAYER_Y;
	transform_.position_.z = DUMMY_PLAYER_Z;
	transform_.position_.x = DUMMY_PLAYER_X;
	transform_.scale_ = { DUMMY_PLAYER_SCALE, DUMMY_PLAYER_SCALE, DUMMY_PLAYER_SCALE };
	transform_.rotate_ = { 0,DUMMY_PLAYER_ROTATE,0 };
	state_ = IDLE;
	Model::SetAnimFrame(hModel_, ANIM_START_FRAME, WALK_ANIM_END, WALK_ANIM_SPEED);
	Model::SetAnimFrame(hClearModel_, ANIM_START_FRAME, CLEAR_ANIM_END, CLEAR_ANIM_SPEED);
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
