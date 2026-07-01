#include "FPSCamera.h"
#include "Engine/Camera.h"
#include "Engine/Direct3D.h"
#include "FPSgun.h"

namespace
{
	constexpr float CAMERA_SENSITIVITY = 0.2f;
	constexpr float CAMERA_MAX_PITCH = 80.0f;
	constexpr float CAMERA_HEIGHT = 1.7f;
}

FPSCamera::FPSCamera(GameObject* parent)
	:GameObject(parent, "FPSCamera"), DeltaX(0), DeltaY(0), currentMousePos({ 0,0,0 }), isPlay(true), isCursorHidden(false)
{
	centerX = Direct3D::screenWidth_ / 2;//スタート時のマウス座標を画面中央に設定
	centerY = Direct3D::screenHeight_ / 2;//

	currentMousePos = { (float)centerX,(float)centerY,0 };
}

FPSCamera::~FPSCamera()
{
}

void FPSCamera::Initialize()
{
	SetCursorPos(centerX, centerY);
}

void FPSCamera::Update()
{
	if (!isCursorHidden)
	{
		ShowCursor(FALSE);
		isCursorHidden = true;
	}

	POINT pt;
	GetCursorPos(&pt);

	
	DeltaY = pt.y - centerY;
	DeltaX = pt.x - centerX;
	
	HWND hwnd = GetForegroundWindow();

	SetCursorPos(centerX, centerY);

}

void FPSCamera::Draw()
{
}

void FPSCamera::Release()
{
}

void FPSCamera::SetFpsCamera(Transform &cam,float sensitivity)
{
	if (isPlay)
	{

		XMVECTOR vPos = XMLoadFloat3(&cam.position_);

		cam.rotate_.y += DeltaX * sensitivity;
		cam.rotate_.x += DeltaY * sensitivity;

		if (cam.rotate_.x > CAMERA_MAX_PITCH)
		{
			cam.rotate_.x = CAMERA_MAX_PITCH;
		}

		if (cam.rotate_.x < -CAMERA_MAX_PITCH)
		{
			cam.rotate_.x = -CAMERA_MAX_PITCH;
		}

		XMMATRIX camRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(cam.rotate_.x), XMConvertToRadians(cam.rotate_.y), 0);


		XMVECTOR vCam = { 0,CAMERA_HEIGHT, 0, 0 };//カメラ位置ベクトル

		XMVECTOR forward = XMVector3TransformNormal(XMVectorSet(0, 0, 1, 0), camRotate);

		//カメラ位置設定
		XMFLOAT3 camPos;
		XMStoreFloat3(&camPos, vPos + vCam);

		Camera::SetPosition(camPos);

		//カメラターゲット設定
		XMFLOAT3 camTarget;
		XMStoreFloat3(&camTarget, vPos + vCam + forward);

		Camera::SetTarget(camTarget);
	}
}
