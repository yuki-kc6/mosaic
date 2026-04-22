#include "BuildingManager.h"
#include "Building.h"	
#include "Engine/Model.h"

BuildingManager::BuildingManager(GameObject* parent)
{
}

BuildingManager::~BuildingManager()
{
}

void BuildingManager::Initialize()
{
	for (int i = 0; i < 10; i++) {
		Building* building = new Building(this);
		building->Initialize();
		building->SetPosition(XMFLOAT3(i * 10.0f, 0, 50.0f));
	}

}

void BuildingManager::Update()
{
}

void BuildingManager::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void BuildingManager::Release()
{
}
