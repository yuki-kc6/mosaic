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
	const XMFLOAT3 CAMERA_START_POS = { 200.0f, 10.0f, -70.0f };
	constexpr float CAMERA_TARGET_HEIGHT = 5.0f;
	constexpr float CAMERA_MOVE_LERP = 0.05f;
	constexpr float CAMERA_ROTATE_LERP = 0.03f;
	constexpr float CAMERA_LOOK_DISTANCE = 10.0f;
	constexpr float SCENE_CHANGE_DISTANCE = 0.1f;
	constexpr float ROTATE_SPEED = 0.5f;  // 1フレームに何度回るか
	constexpr float TARGET_ANGLE = 90.0f; // 真後ろ
}


//コンストラクタ
TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene")
	, currentAngleY(0.0f)
{

}

//初期化
void TitleScene::Initialize()
{
	Instantiate<Ground>(this);

	Instantiate<StageManager>(this);
	Instantiate<NPCManager>(this);
	Instantiate<DummyPlayer>(this);

	Instantiate<TitleHeader>(this);

	hTitlePic_ = Image::Load("titleName.png");

	isStart = false;

	XMFLOAT3 dummyPlayerPos = FindObject("DummyPlayer")->GetPosition();

	Camera::SetPosition(CAMERA_START_POS);

	Camera::SetTarget(dummyPlayerPos);
	currentPos = Camera::GetPosition();


	targetPos = {dummyPlayerPos.x,CAMERA_TARGET_HEIGHT,dummyPlayerPos.z};
}

//更新
void TitleScene::Update()
{

	if (Input::IsKeyDown(DIK_SPACE))
	{
		if(isStart=false)
			FindObject("TitleHeader")->KillMe();
		isStart = true;
		
	}

	if (isStart)
	{
		// 位置のLerp
		XMVECTOR vCurrentPos = XMLoadFloat3(&currentPos);
		XMVECTOR vTargetPos = XMLoadFloat3(&targetPos);
		vCurrentPos = XMVectorLerp(vCurrentPos, vTargetPos, CAMERA_MOVE_LERP);
		XMStoreFloat3(&currentPos, vCurrentPos);

		//カメラを回転させる
		currentAngleY += (TARGET_ANGLE - currentAngleY) * CAMERA_ROTATE_LERP;
		XMVECTOR vDir = XMVector3TransformNormal(
			XMVectorSet(0, 0, 1, 0),
			XMMatrixRotationY(XMConvertToRadians(currentAngleY))
			);

		XMFLOAT3 newLook;
		XMStoreFloat3(&newLook, vTargetPos + vDir * CAMERA_LOOK_DISTANCE);
		currentLook = newLook;

		Camera::SetPosition(currentPos);
		Camera::SetTarget(currentLook);


		XMVECTOR vDiff = vTargetPos - vCurrentPos;
		float dist = XMVectorGetX(XMVector3Length(vDiff));

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
