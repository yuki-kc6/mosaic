#include "DummyPlayer.h"
#include "Player.h"
#include "Engine/Model.h"

namespace
{
	constexpr float DUMMY_PLAYER_SCALE = 0.05f;//モデルのスケール

	constexpr XMFLOAT3 DUMMY_PLAYER_POSITION = { 70.0f,1.0f,-70.0f };//モデルの座標

	constexpr float DUMMY_PLAYER_ROTATE = 90.0f;//モデルの回転角度
	constexpr float ANIM_START_FRAME = 0.0f;//アニメーション開始フレーム

	constexpr float IDLE_ANIM_END = 60.0f;//待機アニメーション終了フレーム
	constexpr float IDLE_ANIM_SPEED = 1.0f;//待機アニメーション速度
	constexpr float CLEAR_ANIM_END = 30.0f;//クリア時のモーション終了フレーム
	constexpr float CLEAR_ANIM_SPEED = 0.75f;//クリア時のモーション速度
}

DummyPlayer::DummyPlayer(GameObject* parent)
	:GameObject(parent, "DummyPlayer"), state_(IDLE),hClearModel_(0)
{
}

DummyPlayer::~DummyPlayer()
{
}

void DummyPlayer::Initialize()
{
	hModel_ = Model::Load("Models/Player.fbx");//待機時のモデル
	hClearModel_ = Model::Load("Models/Player_ClearDance.fbx");//クリア時のモーション用モデル

	transform_.position_ = DUMMY_PLAYER_POSITION;//モデルの初期座標
	transform_.scale_ = { DUMMY_PLAYER_SCALE, DUMMY_PLAYER_SCALE, DUMMY_PLAYER_SCALE };//モデルのスケール
	transform_.rotate_ = { 0,DUMMY_PLAYER_ROTATE,0 };//モデルの回転角度

	Model::SetAnimFrame(hModel_, ANIM_START_FRAME,IDLE_ANIM_END,IDLE_ANIM_SPEED);//待機時のアニメーション開始フレーム
	Model::SetAnimFrame(hClearModel_, ANIM_START_FRAME,CLEAR_ANIM_END,CLEAR_ANIM_SPEED);//クリア時のアニメーション開始フレーム

	state_ = IDLE;//初期状態は待機状態


	//親がPlaySceneの時は最初非表示にしてあげる
	if (GetParent()->GetObjectName() == "PlayScene") {
		Invisible();
	}

}

void DummyPlayer::Update()
{
	//親がPlaySceneの時はプレイヤーの座標に追従する
	if (GetParent()->GetObjectName() == "PlayScene") {
		transform_.position_=FindObject("Player")->GetPosition();
	}

	
}

void DummyPlayer::Draw()
{
	//状態に応じてモデルを描画する
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
