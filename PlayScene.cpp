#include "PlayScene.h"
#include "Ground.h"
#include "Player.h"
#include "Enemy.h"
#include "MosaicPrinter.h"
#include "Wall.h"
#include "Engine/SceneManager.h"
#include "StageTimer.h"
#include "Building.h"
#include "FPSgun.h"
#include "StageManager.h"
#include "DummyPlayer.h"
#include "NPCManager.h"
#include "Engine/Audio.h"
#include "Engine/Camera.h"


//コンストラクタ
PlayScene::PlayScene(GameObject * parent)
	: GameObject(parent, "PlayScene")
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


	this->PushSensitive();
}

//更新
void PlayScene::Update()
{
	this->MissionAllClear();

	StageTimer* timer=(StageTimer*)FindObject("StageTimer");
	if (timer != nullptr) {
		if (timer->GetIsTimeOver())
		{
			TimerOverEffect();
		}
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

void PlayScene::MissionAllClear()
{
	bool isClear=false;
    PaintObject* closestObj = nullptr;
    for (PaintObject* pObj : sensitiveList_) {
		if (!pObj->IsClear())
		{ 
			isClear = false;
			break;
		}
		isClear = true;
    }
    if (isClear)
    {
		ClearEffect();
		//SceneManager* sm = (SceneManager*)FindObject("SceneManager");
		//sm->ChangeScene(SCENE_ID_RESULT);
    }
}

void PlayScene::TimerOverEffect()
{
	
}

void PlayScene::ClearEffect()
{
	

	FPSCamera* fpsCamera=(FPSCamera*)FindObject("FPSCamera");
	if (fpsCamera) {
		fpsCamera->SetIsPlay(false);
	}

	FPSgun* fpsGun = (FPSgun*)FindObject("FPSgun");
	if (fpsGun) {
		fpsGun->KillMe();
	}
	Player* player = (Player*)FindObject("Player");
	if (player) {
		player->SetIsPlay(false);
	}
	DummyPlayer* dummyPlayer = (DummyPlayer*)FindObject("DummyPlayer");
	if (dummyPlayer) {
		dummyPlayer->Visible();
	}
	StageTimer* timer = (StageTimer*)FindObject("StageTimer");
	if (timer) {
		timer->Invisible();
	}



	XMFLOAT3 offset = { 1.0f, 1.0f, -2.5f };

	XMFLOAT3 currentCameraPos=	Camera::GetPosition();
	XMFLOAT3 currentCameraTarget = Camera::GetTarget();

	XMVECTOR vCurrentCameraPos = XMLoadFloat3(&currentCameraPos);
	XMVECTOR vCurrentCameraTarget = XMLoadFloat3(&currentCameraTarget);

	XMFLOAT3 playerPos = player->GetPosition();
	XMFLOAT3 cameraPos;

	XMVECTOR vPlayerPos;
	XMVECTOR vCameraPos;
	

	vCameraPos = XMVectorLerp(vCurrentCameraPos, vCurrentCameraTarget, 0.05f);

	XMFLOAT3 cameraTarget = player->GetPosition();
	cameraTarget.y += 1.5f;   // 顔あたり
	cameraTarget.z -= 3.0f;   // プレイヤーの後ろ

	XMVECTOR vTargetPos = XMLoadFloat3(&cameraTarget);

	vCameraPos = XMVectorLerp(
		vCurrentCameraPos,
		vTargetPos,
		0.05f
	);


	XMStoreFloat3(&cameraPos, vCameraPos);

	Camera::SetPosition(cameraPos);
	Camera::SetTarget(cameraTarget);











}
