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

	for (int i = 0; i < csv.GetWidth();i++)
	{
		std::vector<int> mapsLine;
		for (int j = 0; j < csv.GetHeight(); j++)
		{
			mapsLine.push_back(csv.GetValue(i, j));
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

