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
	:GameObject(parent, "StageManager"),gridSize_(0)
{

}

StageManager::~StageManager()
{

}

void StageManager::Initialize()
{
	gridSize_ = GRID_SIZE;
	CsvReader csv;
	csv.Load("StageData.csv");
	maps_.clear();

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

		maps_.push_back(mapsLine);
	}
	BuildStage();
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
	//マップが正しく入っているなら返す

	if (z < 0 || z >= maps_.size())
		return -1;

	if (x < 0 || x >= maps_[z].size())
		return -1;

	return maps_[z][x];
}

void StageManager::BuildStage()
{
	for (int z = 0; z < maps_.size(); z++)
	{
		for (int x = 0; x < maps_[z].size(); x++)
		{
			if (maps_[z][x] == MAP_SIDEWALK)
			{
				//歩道、特に模様などなし
			}
			if (maps_[z][x] == MAP_BUILDING)
			{
				//ビル
				Instantiate<Building>(this)->SetPosition(x * gridSize_, 0.0f, -z * gridSize_);
				buildingList_.push_back({ z,x });

			}
			if (maps_[z][x] == MAP_ROAD)
			{
				//道路
			}
			if (maps_[z][x] == MAP_CROSSWALK)
			{
				//横断歩道
			}
		}
	}
}

