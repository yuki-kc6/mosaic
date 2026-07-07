#include "FPSCamera.h"
#include "Engine/Camera.h"
#include "Engine/Direct3D.h"
#include "FPSgun.h"

namespace
{
	constexpr float CAMERA_SENSITIVITY = 0.2f;//カメラの感度
	constexpr float CAMERA_MAX_PITCH = 80.0f;//カメラの最大ピッチ角度
	constexpr float CAMERA_HEIGHT = 1.7f;//カメラの高さ
}

FPSCamera::FPSCamera(GameObject* parent)
	:GameObject(parent, "FPSCamera"), deltaX_(0), deltaY_(0), isPlay_(true), isCursorHidden_(false)
{
	//スタート時のマウス座標を画面中央に設定
	centerX_ = Direct3D::screenWidth_ / 2;
	centerY_ = Direct3D::screenHeight_ / 2;

}

FPSCamera::~FPSCamera()
{
}

void FPSCamera::Initialize()
{
	SetCursorPos(centerX_, centerY_);//マウスを画面中央に設定
}

void FPSCamera::Update()
{
	if (!isCursorHidden_)
	{
		ShowCursor(FALSE);//カーソルを非表示にする
		isCursorHidden_ = true;
	}

	POINT pt;
	GetCursorPos(&pt);//マウスの座標を取得

	
	deltaY_ = pt.y - centerY_;//マウスのY座標の移動量を計算
	deltaX_ = pt.x - centerX_;//マウスのX座標の移動量を計算
	
	HWND hwnd = GetForegroundWindow();//現在アクティブなウィンドウのハンドルを取得

	SetCursorPos(centerX_, centerY_);//マウスを画面中央に戻す

}

void FPSCamera::Draw()
{
}

void FPSCamera::Release()
{
}

void FPSCamera::SetFpsCamera(Transform &transform,float sensitivity)
{
	if (isPlay_)
	{
		//プレイヤーの位置を取得
		XMVECTOR vPos = XMLoadFloat3(&transform.position_);

		//マウスの移動量をカメラの回転に反映
		transform.rotate_.y += deltaX_ * sensitivity;
		transform.rotate_.x += deltaY_ * sensitivity;

		//カメラのピッチ角度を制限
		if (transform.rotate_.x > CAMERA_MAX_PITCH)
		{
			transform.rotate_.x = CAMERA_MAX_PITCH;
		}
		if (transform.rotate_.x < -CAMERA_MAX_PITCH)
		{
			transform.rotate_.x = -CAMERA_MAX_PITCH;
		}

		//カメラの回転行列を作成
		XMMATRIX camRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(transform.rotate_.x), XMConvertToRadians(transform.rotate_.y), 0);


		XMVECTOR vCam = { 0,CAMERA_HEIGHT, 0, 0 };//カメラ位置ベクトル

		XMVECTOR forward = XMVector3TransformNormal(XMVectorSet(0, 0, 1, 0), camRotate);//カメラの前方向ベクトル

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
