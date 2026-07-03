#include "NPCManager.h"
#include "Engine/Model.h"
#include "StageManager.h"
#include "Enemy.h"

namespace
{
	const int MAX_NPC = 7;
}

NPCManager::NPCManager(GameObject* parent)
	:GameObject(parent, "NPCManager")
{

}

NPCManager::~NPCManager()
{

}

void NPCManager::Initialize()
{
	stageManager = (StageManager*)FindObject("StageManager");
	spawnList = stageManager->GetBuildingList();

	gridSize = stageManager->GetGridSize();

    SpawnNPC();
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
    int npcCount = 0;

    for (int i = 0; i < MAX_NPC && !spawnList.empty(); i++)
    {
        int index = rand() % spawnList.size();
        int x = spawnList[index].second;
        int z = spawnList[index].first;

        Instantiate<Enemy>(this)->SetPosition(
            x * gridSize,
            0.0f,
            -z * gridSize);

        // 同じ場所に生成されないよう削除
        spawnList.erase(spawnList.begin() + index);
    }
}

