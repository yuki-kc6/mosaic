#include "StageManager.h"
#include "Building.h"	
#include "Ground.h"
#include "Engine/Model.h"
#include "Engine/CsvReader.h"
#include "Enemy.h"

StageManager::StageManager(GameObject* parent)
	:GameObject(parent, "StageManager")
{

}

StageManager::~StageManager()
{

}

void StageManager::Initialize()
{
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

	for (int z = 0; z < maps.size(); z++)
	{
		for (int x = 0; x < maps[z].size(); x++)
		{
			if (maps[z][x] == 1)
			{
				//ビル
				Instantiate<Building>(this)->SetPosition(x * 29.0f, 0.0f, -z * 29.0f);
				if (a < 2)
				{
					Instantiate<Enemy>(this)->SetPosition(x * 29.0f, 0.0f, -z * 29.0f);
					a++;
				}
			}
			if (maps[z][x] == 0)
			{
				//歩道、特に模様などなし
			}
			if (maps[z][x] == 2)
			{
				//道路、mapの繋がり方でrotateする
			}
			if (maps[z][x] == 3)
			{
				//横断歩道、道路に合わせてrotateする
			}
		}
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

