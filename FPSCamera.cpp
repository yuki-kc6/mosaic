#include "FPSCamera.h"
#include "Engine/Camera.h"
#include "Engine/Direct3D.h"

FPSCamera::FPSCamera():DeltaX(0),DeltaY(0),currentMousePos({0,0,0})
{
	centerX = Direct3D::screenWidth_ / 2;
	centerY = Direct3D::screenHeight_ / 2;
	
	currentMousePos = { (float)centerX,(float)centerY,0 };
}

FPSCamera::~FPSCamera()
{
}

void FPSCamera::Update()
{
	ShowCursor(FALSE);
	POINT pt;
	GetCursorPos(&pt);

	
	DeltaY = pt.y - centerY;
	DeltaX = pt.x - centerX;
	
	HWND hwnd = GetForegroundWindow();

	SetCursorPos(centerX, centerY);

}

void FPSCamera::SetFpsCamera(Transform &cam,float sensitivity)
{
	XMVECTOR vPos = XMLoadFloat3(&cam.position_);

	cam.rotate_.y += DeltaX*sensitivity;
	cam.rotate_.x += DeltaY*sensitivity;

	if (cam.rotate_.x > 80.0f)
	{
		cam.rotate_.x = 80.0f;
	}

	if (cam.rotate_.x < -80.0f)
	{
		cam.rotate_.x = -80.0f;
	}

	XMMATRIX camRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(cam.rotate_.x), XMConvertToRadians(cam.rotate_.y), 0);


	XMVECTOR vCam = { 0,1.7f, 0, 0 };//カメラ位置ベクトル

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
