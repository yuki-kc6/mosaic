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
#include "NPCManager.h"
#include "Engine/Audio.h"

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

	//Instantiate<StageTimer>(this);

	Instantiate<StageManager>(this);
	//Instantiate<NPCManager>(this);
	Instantiate<Player>(this);

	this->PushSensitive();
}

//更新
void PlayScene::Update()
{
	this->MissionAllClear();
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
		if (!pObj->IsClear())continue;
		isClear = pObj->IsClear();
    }
    if (isClear)
    {
		SceneManager* sm = (SceneManager*)FindObject("SceneManager");
		sm->ChangeScene(SCENE_ID_RESULT);
    }
}
