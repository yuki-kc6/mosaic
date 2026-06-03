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
	//SetPosition(XMFLOAT3(0, 0.5, 0));
	SetRotate(XMFLOAT3(0, -80, 0));
}

void FPSgun::Update()
{
    XMFLOAT3 camPos = Camera::GetPosition();
    XMFLOAT3 camTarget = Camera::GetTarget();

    XMVECTOR vCamPos = XMLoadFloat3(&camPos);
    XMVECTOR vCamTarget = XMLoadFloat3(&camTarget);

    XMVECTOR vForward = XMVector3Normalize(vCamTarget - vCamPos);
    XMVECTOR vRight = XMVector3Normalize(XMVector3Cross(vForward, XMVectorSet(0, 1, 0, 0)));
    XMVECTOR vUp = XMVector3Normalize(XMVector3Cross(vRight, vForward));

    // この3つの値で画面上の位置を調整
    XMVECTOR vGunPos = vCamPos
        + vForward * 3.0f   // 前方向
        + vRight * 0.8f   // 右方向
        + vUp * -0.5f; // 上下方向

    XMFLOAT3 gunPos;
    XMStoreFloat3(&gunPos, vGunPos);
    SetPosition(gunPos);
}

void FPSgun::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void FPSgun::Release()
{
}
