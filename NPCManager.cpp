#include "NPCManager.h"
#include "Engine/Model.h"
#include "StageManager.h"
#include "Enemy.h"


NPCManager::NPCManager(GameObject* parent)
{

}

NPCManager::~NPCManager()
{

}

void NPCManager::Initialize()
{
	stageManager = (StageManager*)FindObject("StageManager");
	Instantiate<Enemy>(this)->SetPosition(0.0f, 0.0f, 0.0f);
}

void NPCManager::Update()
{
	//一定時間経過、k
	//vectorにNPCを登録
	//ビルの隣のマスにスポーン
	//Instantiate<Enemy>(this)->SetPosition(x * 29.0f, 0.0f, -z * 29.0f);
	
}

void NPCManager::Draw()
{

}

void NPCManager::Release()
{
}

