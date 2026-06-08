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
	//transform_.rotate_.y = 100.0f; // モデルの向きを調整
    transform_.pParent_ = nullptr;
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

    // 位置をカメラ基準で固定
    XMVECTOR vGunPos = vCamPos
        + vForward * 3.0f
		+ vRight *-1.0f
        + vUp * -0.6f;

    XMFLOAT3 gunPos;
    XMStoreFloat3(&gunPos, vGunPos);
    SetPosition(gunPos);

    // ★向きもカメラに追従させる
    GameObject* pParent = GetParent();
    XMFLOAT3 ro = pParent->GetRotate();
    transform_.rotate_.y = ro.y;
    transform_.rotate_.x = ro.x;
}

void FPSgun::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void FPSgun::Release()
{
}
