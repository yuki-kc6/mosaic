#include "StageManager.h"
#include "Building.h"	
#include "Ground.h"
#include "Engine/Model.h"
#include "Engine/CsvReader.h"
#include "Enemy.h"

namespace
{
	const float GRID_SIZE = 29.0f;
}

StageManager::StageManager(GameObject* parent)
	:GameObject(parent, "StageManager")
{

}

StageManager::~StageManager()
{

}

void StageManager::Initialize()
{
	gridSize = GRID_SIZE;
	CsvReader csv;
	csv.Load("StageData.csv");
	maps.clear();

	int w = csv.GetWidth();
	int h = csv.GetHeight();

	for (int y = 0; y < h; y++)
	{
		std::vector<int> mapsLine;
		mapsLine.clear();
		for (int x = 0; x < w; x++)
		{

			mapsLine.push_back(csv.GetValue(x, y));

		}

		maps.push_back(mapsLine);
	}
}

void StageManager::Update()
{
}

void StageManager::Draw()
{

}

void StageManager::Release()
{
}

int StageManager::GetMap(int x, int z)
{
	if (z < 0 || z >= maps.size())
		return -1;

	if (x < 0 || x >= maps[z].size())
		return -1;

	return maps[z][x];
}

void StageManager::BuildStage()
{
	for (int z = 0; z < maps.size(); z++)
	{
		for (int x = 0; x < maps[z].size(); x++)
		{
			if (maps[z][x] == MAP_SIDEWALK)
			{
				//歩道、特に模様などなし
			}
			if (maps[z][x] == MAP_BUILDING)
			{
				//ビル
				Instantiate<Building>(this)->SetPosition(x * gridSize, 0.0f, -z * gridSize);
				buildingList.push_back({ z,x });

			}
			if (maps[z][x] == MAP_ROAD)
			{
				//道路
			}
			if (maps[z][x] == MAP_CROSSWALK)
			{
				//横断歩道
			}
		}
	}
}

