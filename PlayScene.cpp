#include "PlayScene.h"
#include "Ground.h"
#include "Player.h"
#include "MosaicPrinter.h"
#include "Engine/SceneManager.h"
#include "StageTimer.h"
#include "Building.h"
#include "FPSgun.h"
#include "StageManager.h"
#include "DummyPlayer.h"
#include "NPCManager.h"
#include "Engine/Audio.h"
#include "Engine/Camera.h"
#include "GrayOut.h"
#include "ClearEffect.h"


namespace
{
	constexpr float END_CAMERA_DISTANCE = 30.0f;
	constexpr float END_CAMERA_HEIGHT = 10.0f;
	constexpr float END_CAMERA_TARGET_HEIGHT = 3.0f;

	constexpr float END_CAMERA_MOVE_LERP = 0.03f;
	constexpr float END_CAMERA_TARGET_LERP = 0.01f;
}


//コンストラクタ
PlayScene::PlayScene(GameObject * parent)
	: GameObject(parent, "PlayScene"),isEndCameraStarted(false), isGameOver(false)
{
}

//初期化
void PlayScene::Initialize()
{
	

	MosaicPrinter::Initialize();
	
	Instantiate<Ground>(this);	

	Instantiate<StageManager>(this);
	Instantiate<NPCManager>(this);
	Instantiate<DummyPlayer>(this);

	Instantiate<StageTimer>(this);

	Instantiate<Player>(this);

	Instantiate<GrayOut>(this);

	Instantiate<ClearEffect>(this);

	this->PushSensitive();

	
}

//更新
void PlayScene::Update()
{
	if (this->CheckMissionClear() || this->CheckTimeOver())
	{
		if (!isEndCameraStarted) {
			StartEndCamera();

		}
		UpdateEndCamera();
		
	}


}

//描画
void PlayScene::Draw()
{
	

}

//開放
void PlayScene::Release()
{
	MosaicPrinter::Release();
	Audio::Release();
}

void PlayScene::PushSensitive()
{
    for (PaintObject* pObj : PaintObject::GetPaintObjectList()) {
        // 描画されていない、または死んでいるオブジェクトはスキップ
		if (!pObj->IsVisibled() || pObj->IsDead()) continue;
		if (!pObj->IsSensitive())continue;
		sensitiveList_.push_back(pObj);
    }

}

bool PlayScene::CheckMissionClear()
{
	bool isClear = false;
    PaintObject* closestObj = nullptr;
    for (PaintObject* pObj : sensitiveList_) {
		if (!pObj->IsClear())
		{ 
			isClear = false;
			break;
		}
		isClear = true;
    }
	return isClear;
}

bool PlayScene::CheckTimeOver()
{
	bool isTimeOver = false;
	StageTimer* timer = (StageTimer*)FindObject("StageTimer");
	if (timer != nullptr) {
		if (timer->GetIsTimeOver())
		{
			isTimeOver = timer->GetIsTimeOver();
		}
	}
	return isTimeOver;
}

void PlayScene::TimerOverEffect()
{
	
}



void PlayScene::StartEndCamera()
{
	isGameOver = CheckTimeOver();
	isMissionClear = CheckMissionClear();

	FPSCamera* fpsCamera = (FPSCamera*)FindObject("FPSCamera");
	if (fpsCamera) {
		fpsCamera->SetIsPlay(false);
	}

	FPSgun* fpsGun = (FPSgun*)FindObject("FPSgun");
	if (fpsGun) {
		fpsGun->KillMe();
	}
	Player* player = (Player*)FindObject("Player");
	if (player) {
		player->Invisible();
		player->SetIsPlay(false);
	}
	DummyPlayer* dummyPlayer = (DummyPlayer*)FindObject("DummyPlayer");
	if (dummyPlayer) {
		dummyPlayer->Visible();
		if (isMissionClear)
		{
			dummyPlayer->SetState(PlayerState::CLEAR);

		}
	}
	StageTimer* timer = (StageTimer*)FindObject("StageTimer");
	if (timer) {
		timer->SetTimer(false);
		timer->Invisible();
	}

	
	GrayOut* grayOut = (GrayOut*)FindObject("GrayOut");
	if (grayOut) {
		grayOut->SetTimeOver(isGameOver);
	}

	
	ClearEffect* clearEffect = (ClearEffect*)FindObject("ClearEffect");
	if(clearEffect) {
		clearEffect->SetEffectActive(isMissionClear);
	}

	float rotY = player->GetRotate().y;
	float rad = XMConvertToRadians(rotY);

	XMFLOAT3 playerPos = player->GetPosition();


	endCameraPos.x = playerPos.x - sinf(rad) * END_CAMERA_DISTANCE;
	endCameraPos.y = playerPos.y + END_CAMERA_HEIGHT;
	endCameraPos.z = playerPos.z - cosf(rad) * END_CAMERA_DISTANCE;


	endCameraTarget.x = playerPos.x;
	endCameraTarget.y = playerPos.y + END_CAMERA_TARGET_HEIGHT;
	endCameraTarget.z = playerPos.z;
}

void PlayScene::UpdateEndCamera()
{

	XMFLOAT3 currentCameraPos = Camera::GetPosition();
	XMFLOAT3 currentCameraTarget = Camera::GetTarget();

	XMVECTOR vCurrentCameraPos = XMLoadFloat3(&currentCameraPos);
	XMVECTOR vCurrentCameraTarget = XMLoadFloat3(&currentCameraTarget);

	XMVECTOR vEndCameraPos = XMLoadFloat3(&endCameraPos);
	XMVECTOR vEndCameraTarget = XMLoadFloat3(&endCameraTarget);


	XMVECTOR vCameraPos;
	XMVECTOR vCameraTarget;

	vCameraPos = XMVectorLerp(vCurrentCameraPos, vEndCameraPos, END_CAMERA_MOVE_LERP);
	vCameraTarget = XMVectorLerp(vCurrentCameraTarget, vEndCameraTarget, END_CAMERA_TARGET_LERP);

	XMFLOAT3 cameraPos;
	XMFLOAT3 cameraTarget;
	XMStoreFloat3(&cameraPos, vCameraPos);
	XMStoreFloat3(&cameraTarget, vCameraTarget);

	Camera::SetPosition(cameraPos);
	Camera::SetTarget(cameraTarget);

	endFrame++;

	if (endFrame >= kTitleChangeFrame)
	{
		ClearEffect* clearEffect = (ClearEffect*)FindObject("ClearEffect");
		clearEffect->EffectEnd();

		SceneManager* sm = (SceneManager*)FindObject("SceneManager");
		sm->ChangeScene(SCENE_ID_TITLE);
	}

}
