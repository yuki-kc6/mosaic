#include "building.h"

#include "Engine/BoxCollider.h"
#include "Engine/Model.h"
#include "Engine/Direct3D.h"
#include "RenderTexture.h"

Building::Building(GameObject* parent)
	:PaintObject(parent, "Building")
{
}

Building::~Building()
{
}

void Building::Initialize()
{
    hModel_ = Model::Load("Models/building.fbx");
    assert(hModel_ >= 0);
    transform_.position_.y = 10.;
    this->SetSensitive(true);
    BoxCollider* collision = new BoxCollider(XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1));
    AddCollider(collision);
}

void Building::Update()
{
}

void Building::Draw()
{
    Model::SetTransform(hModel_, transform_, this->GetMosaicRT()->GetShaderResourceView());
    Model::Draw(hModel_);
}

void Building::Release()
{
}
