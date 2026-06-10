#include "TitleScene.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "TitleHeader.h"
#include "StageManager.h"
#include "DummyPlayer.h"
#include "Engine/Camera.h"
#include "Ground.h"

//コンストラクタ
TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene")
{

}

//初期化
void TitleScene::Initialize()
{
	Instantiate<Ground>(this);
	Instantiate<StageManager>(this);
	Instantiate<DummyPlayer>(this);
	
	isStart = false;

	Camera::SetPosition({ 200, 10, -70 });

	Camera::SetTarget(FindObject("DummyPlayer")->GetPosition());

}

//更新
void TitleScene::Update()
{
	camPos = Camera::GetPosition();
	camTargetPos= Camera::GetTarget();
	vCamPos = XMLoadFloat3(&camPos);
	vCamTargetPos = XMLoadFloat3(&camTargetPos);

	if (Input::IsKeyDown(DIK_SPACE))
	{
		isStart = true;

		//SceneManager* sm = (SceneManager*)FindObject("SceneManager");
		//sm->ChangeScene(SCENE_ID_PLAY);
	}

	if (isStart)
	{
		vCamPos = XMVectorLerp(vCamPos, vCamTargetPos, 0.05f);

		XMStoreFloat3(&camPos, vCamPos);

		Camera::SetPosition(camPos);


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
