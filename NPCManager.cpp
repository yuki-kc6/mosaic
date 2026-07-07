#include "NPCManager.h"
#include "Engine/Model.h"
#include "StageManager.h"
#include "Enemy.h"

namespace
{
	const int MAX_NPC = 1;//最大生成数
}

NPCManager::NPCManager(GameObject* parent)
	:GameObject(parent, "NPCManager"),stageManager(nullptr),gridSize(0.0f),spawnList()
{

}

NPCManager::~NPCManager()
{

}

void NPCManager::Initialize()
{
	stageManager = (StageManager*)FindObject("StageManager");
	spawnList = stageManager->GetBuildingList();//ビルの座標をスポーンリストにコピー

	gridSize = stageManager->GetGridSize();//ステージのグリッドサイズを取得

	SpawnNPC();//NPCを生成
}

void NPCManager::Update()
{
	
	
}

void NPCManager::Draw()
{

}

void NPCManager::Release()
{
}

void NPCManager::SpawnNPC()
{
	int npcCount = 0;//生成したNPCの数

	//ランダムにNPCを生成する
    for (int i = 0; i < MAX_NPC && !spawnList.empty(); i++)
    {
		int index = rand() % spawnList.size();//ランダムにスポーンリストからインデックスを取得
        int x = spawnList[index].second;
        int z = spawnList[index].first;

        Instantiate<Enemy>(this)->SetPosition(x * gridSize,0.0f,-z * gridSize);//NPC生成

        // 同じ場所に生成されないよう削除
        spawnList.erase(spawnList.begin() + index);
    }
}

