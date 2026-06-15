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
	Instantiate<Ground>(this);

	Instantiate<StageManager>(this);
	Instantiate<DummyPlayer>(this);

	Instantiate<TitleHeader>(this);

	hTitlePic_ = Image::Load("titleName.png");

	isStart = false;

	XMFLOAT3 pos = FindObject("DummyPlayer")->GetPosition();

	Camera::SetPosition({ 200, 10, -70 });

	Camera::SetTarget(pos);

	currentPos = Camera::GetPosition();

	

	targetPos = {
		pos.x,
		5,
		pos.z
	};
}

//更新
void TitleScene::Update()
{

	if (Input::IsKeyDown(DIK_SPACE))
	{
		isStart = true;
		FindObject("TitleHeader")->KillMe();
	}

	if (isStart)
	{
		// 位置のLerp
		XMVECTOR vCurrentPos = XMLoadFloat3(&currentPos);
		XMVECTOR vTargetPos = XMLoadFloat3(&targetPos);
		vCurrentPos = XMVectorLerp(vCurrentPos, vTargetPos, 0.05f);
		XMStoreFloat3(&currentPos, vCurrentPos);


		currentAngleY += (targetAngle - currentAngleY) * 0.03f;

		XMVECTOR vDir = XMVector3TransformNormal(
			XMVectorSet(0, 0, 1, 0),
			XMMatrixRotationY(XMConvertToRadians(currentAngleY))
			);

		XMFLOAT3 newLook;
		XMStoreFloat3(&newLook, vTargetPos + vDir * 10.0f);
		currentLook = newLook;

		Camera::SetPosition(currentPos);
		Camera::SetTarget(currentLook);


		XMVECTOR vDiff = vTargetPos - vCurrentPos;
		float dist = XMVectorGetX(XMVector3Length(vDiff));

		if (dist < 0.1f)
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
