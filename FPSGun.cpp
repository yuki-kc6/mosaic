#include "FPSGun.h"
#include "Engine/Model.h"
#include "Engine/Camera.h"
#include "Engine/Audio.h"

namespace
{
    constexpr float GUN_SCALE = 0.2f;//銃の拡大率
	constexpr float CAMERA_HEIGHT = 1.7f;//カメラの高さ

	constexpr float EFFECT_SPEED = 3.0f;//エフェクトの速度
	constexpr float EFFECT_ACCEL = 1.0f;//エフェクトの加速度
	constexpr int EFFECT_DELAY = 0;//エフェクトの遅延
	constexpr XMFLOAT3 EFFECT_DIRECTION_RND = { 1.0f, 1.0f, 1.0f };//エフェクトの方向のランダム範囲

	constexpr float GUN_FORWARD_OFFSET = 3.0f;//銃の前方向へのオフセット
	constexpr float GUN_RIGHT_OFFSET = -1.0f;//銃の右方向へのオフセット
	constexpr float GUN_UP_OFFSET = -0.6f;//銃の上方向へのオフセット

}

FPSgun::FPSgun(GameObject* parent)
	:GameObject(parent, "FPSgun"),hModel_(-1),hEffect_(-1),hSound_(-1)
{

}

FPSgun::~FPSgun()
{
}

void FPSgun::Initialize()
{
	hModel_ = Model::Load("Models/gun.fbx");
   //hSound_ = Audio::Load("", false);
	SetScale(XMFLOAT3(GUN_SCALE,GUN_SCALE,GUN_SCALE));

	transform_.pParent_ = nullptr;//親の影響を受けないようにする
	// エフェクトの初期化
	effectData_.textureFileName = "cloudA.png";
    effectData_.speed = EFFECT_SPEED;
    effectData_.accel = EFFECT_ACCEL;
    effectData_.delay = EFFECT_DELAY;
    effectData_.directionRnd = EFFECT_DIRECTION_RND;
}

void FPSgun::Update()
{
	// カメラの位置とターゲットを取得
    XMFLOAT3 camPos = Camera::GetPosition();
    XMFLOAT3 camTarget = Camera::GetTarget();

    XMVECTOR vCamPos = XMLoadFloat3(&camPos);
    XMVECTOR vCamTarget = XMLoadFloat3(&camTarget);

	// カメラの前方向、右方向、上方向を計算
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

    // 向きも追従させるために親オブジェクトに合わせる
    GameObject* pParent = GetParent();
    XMFLOAT3 ro = pParent->GetRotate();
    transform_.rotate_.y = ro.y;
    transform_.rotate_.x = ro.x;

	XMStoreFloat3(&effectData_.direction, vForward);// エフェクトの方向をカメラの前方向に設定

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
	// エフェクトがすでに開始されている場合は終了する
    if (hEffect_ != -1)
    {
        VFX::End(hEffect_);
    }
	effectData_.position = GetMazzlePosition();// 銃口の位置をエフェクトの位置に設定

    //Audio::Play(hSound_);//音も鳴らす
	hEffect_ = VFX::Start(effectData_);// エフェクトを開始
}

XMFLOAT3 FPSgun::GetMazzlePosition()
{
	// カメラの位置とターゲットを取得
    XMFLOAT3 camPos = Camera::GetPosition();
    XMFLOAT3 camTarget = Camera::GetTarget();

    XMVECTOR vCamPos = XMLoadFloat3(&camPos);
    XMVECTOR vCamTarget = XMLoadFloat3(&camTarget);

	XMVECTOR vForward = XMVector3Normalize(vCamTarget - vCamPos);// カメラの前方向を計算

    // 銃口の位置 = 銃の位置 + 前方向に少し足す
    XMVECTOR vMuzzle = XMLoadFloat3(&transform_.position_) + vForward * 1.0f;

    XMFLOAT3 muzzlePos;
    XMStoreFloat3(&muzzlePos, vMuzzle);
    return muzzlePos;
}
