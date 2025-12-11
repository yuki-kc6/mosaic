#include "Decal.h"

Decal::Decal(GameObject* parent)
	:GameObject(parent, "Decal"), hModel_(-1)
{

}

Decal::~Decal()
{
}

void Decal::Initialize()
{
	this->SetScale(20, 20, 20);
}

void Decal::Update()
{
}

void Decal::Draw()
{
}

void Decal::Release()
{
}
