#include "TitleScene.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "TitleHeader.h"
#include "StageManager.h"
#include "DummyPlayer.h"
#include "Engine/Camera.h"
#include "Ground.h"
#include "Engine/Image.h"

//コンストラクタ
TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene")
{

}

//初期化
void TitleScene::Initialize()
{
	Instantiate<TitleHeader>(this);
	Instantiate<Ground>(this);

	Instantiate<StageManager>(this);
	Instantiate<DummyPlayer>(this);
	
	isStart = false;

	Camera::SetPosition({ 200, 10, -70 });

	Camera::SetTarget(FindObject("DummyPlayer")->GetPosition());

	hTitlePic_ = Image::Load("titleName.png");

	camPos = Camera::GetPosition();
	currentPos = Camera::GetPosition();

	camTargetPos = Camera::GetTarget();
	targetPos = Camera::GetTarget();

	currentLook = Camera::GetTarget();

	targetLook = Camera::GetPosition();
	vCamPos = XMLoadFloat3(&camPos);
	vCamTargetPos = XMLoadFloat3(&camTargetPos);


	XMVECTOR v = { 0,9,0 };
	vCamTargetPos = vCamTargetPos + v;


	
}

//更新
void TitleScene::Update()
{
	

	if (Input::IsKeyDown(DIK_SPACE))
	{
		isStart = true;

		//SceneManager* sm = (SceneManager*)FindObject("SceneManager");
		//sm->ChangeScene(SCENE_ID_PLAY);
	}

	if (isStart)
	{
		// 位置のLerp
		XMVECTOR vCurrentPos = XMLoadFloat3(&currentPos);
		XMVECTOR vTargetPos = XMLoadFloat3(&targetPos);
		vCurrentPos = XMVectorLerp(vCurrentPos, vTargetPos, 0.05f);
		XMStoreFloat3(&currentPos, vCurrentPos);


		// 注視点のLerp（位置と関係なく独立して動く）
		XMVECTOR vCurrentLook = XMLoadFloat3(&currentLook);
		XMVECTOR vTargetLook = XMLoadFloat3(&targetLook); 
		vCurrentLook = XMVectorLerp(vCurrentLook, vTargetLook, 0.01f);
		XMStoreFloat3(&currentLook, vCurrentLook);

		Camera::SetPosition(currentPos);
		Camera::SetTarget(currentLook);


		if (Input::IsKeyDown(DIK_Z))
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
