#include "FPSGun.h"
#include "Engine/Model.h"
#include "Engine/Camera.h"

FPSgun::FPSgun(GameObject* parent)
	:GameObject(parent, "FPSgun"),hModel_(-1)
{

}

FPSgun::~FPSgun()
{
}

void FPSgun::Initialize()
{
	hModel_ = Model::Load("Models/gun.fbx");
	SetScale(XMFLOAT3(0.2f, 0.2f, 0.2f));
	SetPosition(XMFLOAT3(0, 0.5, 0));
	SetRotate(XMFLOAT3(0, -80, 0));
}

void FPSgun::Update()
{
  
}

void FPSgun::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void FPSgun::Release()
{
}
