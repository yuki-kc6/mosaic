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
	SetScale(XMFLOAT3(0.05f, 0.05f, 0.05f));
	//SetPosition(XMFLOAT3(0, 0.5, 0));
	SetRotate(XMFLOAT3(0, 60, 0));
}

void FPSgun::Update()
{
	XMFLOAT3 camPos = Camera::GetPosition();
	XMVECTOR vCamPos = XMLoadFloat3(&camPos);
	XMMATRIX view = Camera::GetViewMatrix();

	XMFLOAT3 camTar = Camera::GetTarget();
	XMVECTOR vCamTar = XMLoadFloat3(&camTar);



	// 逆行列でカメラ空間取得
	XMMATRIX invView = XMMatrixInverse(nullptr, view);

	XMVECTOR right = invView.r[0];
	XMVECTOR up = invView.r[1];
	XMVECTOR forward = invView.r[2];


	XMVECTOR FPSgunPos =
		vCamPos +
		forward * 0.8f +
		right * 0.3f +
		up * -0.2f;

	XMFLOAT3 FPSgunPosFloat3;
	XMStoreFloat3(&FPSgunPosFloat3, FPSgunPos);
	transform_.position_ = FPSgunPosFloat3;
	
	XMVECTOR vGunRot = vCamTar - vCamPos;
	XMFLOAT3 gunRot;
	XMStoreFloat3(&gunRot, vGunRot);

	transform_.rotate_ = gunRot;
	
}

void FPSgun::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void FPSgun::Release()
{
}
