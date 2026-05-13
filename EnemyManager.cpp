#include "EnemyManager.h"
#include "Engine/Model.h"
#include "Engine/CsvReader.h"
#include "Enemy.h"

EnemyManager::EnemyManager(GameObject* parent)
{

}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::Initialize()
{
	CsvReader enemyCsv;
	enemyCsv.Load("CharacterMap.csv");
	enemy.clear();

	int w = enemyCsv.GetWidth();
	int h = enemyCsv.GetHeight();

	for (int y = 0; y < h; y++)
	{
		std::vector<int> enemyLine;
		enemyLine.clear();
		for (int x = 0; x < w; x++)
		{

			enemyLine.push_back(enemyCsv.GetValue(x, y));

		}

		enemy.push_back(enemyLine);
	}

	CsvReader csv;
	csv.Load("StageData.csv");
	maps.clear();

	int mapW = csv.GetWidth();
	int mapH = csv.GetHeight();

	for (int y = 0; y < mapH; y++)
	{
		std::vector<int> mapsLine;
		mapsLine.clear();
		for (int x = 0; x < mapW; x++)
		{

			mapsLine.push_back(csv.GetValue(x, y));

		}

		maps.push_back(mapsLine);
	}

	for (int z = 0; z < enemy.size(); z++)
	{
		for (int x = 0; x < enemy[z].size(); x++)
		{
			if (enemy[z][x] == 1)
			{
				if(maps[z][x] == 0)
				{
					Instantiate<Enemy>(this)->SetPosition(x * 29.0f, 0.0f, -z * 29.0f);
				}
			}
			else if (enemy[z][x] == 0)
			{
			
			}
		}
	}



}

void EnemyManager::Update()
{
}

void EnemyManager::Draw()
{

}

void EnemyManager::Release()
{
}

