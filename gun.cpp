#include "gun.h"
#include "Engine/Model.h"

gun::gun(GameObject* parent)
	:GameObject(parent, "gun"),hModel_(-1)
{

}

gun::~gun()
{
}

void gun::Initialize()
{
	hModel_ = Model::Load("Models/gun.fbx");

}

void gun::Update()
{
}

void gun::Draw()
{
}

void gun::Release()
{
}
