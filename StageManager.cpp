#include "StageManager.h"
#include "Building.h"	
#include "Engine/Model.h"
#include "Engine/CsvReader.h"

StageManager::StageManager(GameObject* parent)
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
				Instantiate<Building>(this)->SetPosition(x * 27.0f, 0.0f, -z * 27.0f);
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

