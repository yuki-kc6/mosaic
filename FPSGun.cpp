#include "FPSGun.h"
#include "Engine/Model.h"
#include "Engine/Camera.h"

namespace
{
    constexpr float GUN_SCALE = 0.2f;
    constexpr float CAMERA_HEIGHT = 1.7f;

    constexpr float EFFECT_SPEED = 3.0f;
    constexpr float EFFECT_ACCEL = 1.0f;
    constexpr int EFFECT_DELAY = 0;

    constexpr XMFLOAT3 EFFECT_DIRECTION_RND = { 1.0f, 1.0f, 1.0f };

    constexpr float GUN_FORWARD_OFFSET = 3.0f;
    constexpr float GUN_RIGHT_OFFSET = -1.0f;
    constexpr float GUN_UP_OFFSET = -0.6f;

}

FPSgun::FPSgun(GameObject* parent)
	:GameObject(parent, "FPSgun"),hModel_(-1),hmit(-1)
{

}

FPSgun::~FPSgun()
{
}

void FPSgun::Initialize()
{
	hModel_ = Model::Load("Models/gun.fbx");
	SetScale(XMFLOAT3(GUN_SCALE,GUN_SCALE,GUN_SCALE));
    transform_.pParent_ = nullptr;
	effectData_.textureFileName = "cloudA.png";
    effectData_.speed = EFFECT_SPEED;
    effectData_.accel = EFFECT_ACCEL;
    effectData_.delay = EFFECT_DELAY;
    effectData_.directionRnd = EFFECT_DIRECTION_RND;
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
        + vForward * GUN_FORWARD_OFFSET
		+ vRight *GUN_RIGHT_OFFSET
        + vUp * GUN_UP_OFFSET;

    XMFLOAT3 gunPos;
    XMStoreFloat3(&gunPos, vGunPos);
    SetPosition(gunPos);

    // 向きもカメラに追従させる
    GameObject* pParent = GetParent();
    XMFLOAT3 ro = pParent->GetRotate();
    transform_.rotate_.y = ro.y;
    transform_.rotate_.x = ro.x;

	XMStoreFloat3(&effectData_.direction, vForward);

}

void FPSgun::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void FPSgun::Release()
{
}

void FPSgun::BangEffect()
{
    if (hmit != -1)
    {
        VFX::End(hmit);
    }
    effectData_.position = GetMazzlePosition();
	hmit=VFX::Start(effectData_);
}

XMFLOAT3 FPSgun::GetMazzlePosition()
{
    XMFLOAT3 camPos = Camera::GetPosition();
    XMFLOAT3 camTarget = Camera::GetTarget();

    XMVECTOR vCamPos = XMLoadFloat3(&camPos);
    XMVECTOR vCamTarget = XMLoadFloat3(&camTarget);

    XMVECTOR vForward = XMVector3Normalize(vCamTarget - vCamPos);

    // ★銃口の位置 = 銃の位置 + 前方向に少しオフセット
    XMVECTOR vMuzzle = XMLoadFloat3(&transform_.position_) + vForward * 1.0f;

    XMFLOAT3 muzzlePos;
    XMStoreFloat3(&muzzlePos, vMuzzle);
    return muzzlePos;
}
