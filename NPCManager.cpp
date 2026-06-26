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
	int mapH = stageManager->GetMapH();
	int mapW = stageManager->GetMapW();
	for (int i = 0;i < mapH;i++)
	{
		for (int j = 0;j < mapW;j++)
		{
			if (stageManager->GetMap(i, j) == 1)
			{
				//Instantiate<Enemy>(this)->SetPosition(j * 29.0f, 0.0f, -i * 29.0f);
			}
		}
	}
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

