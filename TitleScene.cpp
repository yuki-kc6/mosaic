#include "TitleScene.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "TitleHeader.h"
#include "StageManager.h"
#include "NPCManager.h"
#include "DummyPlayer.h"
#include "Engine/Camera.h"
#include "Ground.h"
#include "Engine/Image.h"

namespace
{
	const XMFLOAT3 CAMERA_START_POS = { 200.0f, 10.0f, -70.0f };//カメラの初期位置
	constexpr float CAMERA_TARGET_HEIGHT = 5.0f;//カメラのターゲットの高さ
	constexpr float CAMERA_MOVE_LERP = 0.05f;//カメラの補間率
	constexpr float CAMERA_ROTATE_RATE= 0.03f;//回転の補間率
	constexpr float CAMERA_LOOK_DISTANCE = 10.0f;//カメラの注視点までの距離
	constexpr float SCENE_CHANGE_DISTANCE = 0.1f;//シーン切り替えを行う距離の閾値
	constexpr float ROTATE_SPEED = 0.5f;  // 1フレームに何度回るか
	constexpr float TARGET_ANGLE = 90.0f; // 真後ろ
}


//コンストラクタ
TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene"), currentAngleY_(0.0f),hTitlePic_(-1),cameraCurrentPos_(0,0,0),cameraGoalPos_(0,0,0),cameraCurrentTarget_(0,0,0),isStart_(false)
{

}

//初期化
void TitleScene::Initialize()
{
	//背景用のオブジェクトを呼び出す
	Instantiate<Ground>(this);
	Instantiate<StageManager>(this);
	Instantiate<NPCManager>(this);
	Instantiate<DummyPlayer>(this);

	Instantiate<TitleHeader>(this);


	isStart_ = false;

	XMFLOAT3 dummyPlayerPos = FindObject("DummyPlayer")->GetPosition();//ダミープレイヤーの位置を取得

	//カメラの初期化
	Camera::SetPosition(CAMERA_START_POS);
	Camera::SetTarget(dummyPlayerPos);
	cameraCurrentPos_ = Camera::GetPosition();


	cameraGoalPos_ = {dummyPlayerPos.x,CAMERA_TARGET_HEIGHT,dummyPlayerPos.z};
}

//更新
void TitleScene::Update()
{
	//最初にスペースが押されたら
	if (Input::IsKeyDown(DIK_SPACE))
	{
		//タイトルを消す
		if(isStart_=false)
			FindObject("TitleHeader")->KillMe();
		isStart_ = true;
		
	}

	if (isStart_)
	{
		//カメラ位置をcurrentからgoalへ補完する
		XMVECTOR vCurrentPos = XMLoadFloat3(&cameraCurrentPos_);
		XMVECTOR vGoalPos = XMLoadFloat3(&cameraGoalPos_);
		vCurrentPos = XMVectorLerp(vCurrentPos, vGoalPos, CAMERA_MOVE_LERP);
		XMStoreFloat3(&cameraCurrentPos_, vCurrentPos);

		// カメラの向きを目標角度へ徐々に補間する
		currentAngleY_ += (TARGET_ANGLE - currentAngleY_) * CAMERA_ROTATE_RATE;

		//カメラが向く方向のベクトルを計算する
		XMVECTOR vDir = XMVector3TransformNormal(
			XMVectorSet(0, 0, 1, 0),
			XMMatrixRotationY(XMConvertToRadians(currentAngleY_))
			);

		// 上のベクトルからカメラのターゲットを更新する
		XMFLOAT3 newLook;
		XMStoreFloat3(&newLook, vGoalPos + vDir * CAMERA_LOOK_DISTANCE);
		cameraCurrentTarget_ = newLook;

		Camera::SetPosition(cameraCurrentPos_);
		Camera::SetTarget(cameraCurrentTarget_);

		//距離の計算
		XMVECTOR vDiff = vGoalPos - vCurrentPos;
		float dist = XMVectorGetX(XMVector3Length(vDiff));

		//一定以上近づいたらシーンチェンジ
		if (dist < SCENE_CHANGE_DISTANCE)
		{
			SceneManager* sm = (SceneManager*)FindObject("SceneManager");
			sm->ChangeScene(SCENE_ID_PLAY);
		}
	}

}

//描画
void TitleScene::Draw()
{


}

//開放
void TitleScene::Release()
{

}
