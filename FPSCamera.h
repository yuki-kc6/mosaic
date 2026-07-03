#pragma once
#include <DirectXMath.h>
#include "Engine/Transform.h"
#include "Engine/GameObject.h"

using namespace DirectX;

//FPS視点にするためのクラス
class FPSCamera :public GameObject
{
public:
	//コンストラクタ
	FPSCamera(GameObject* parent);

	//デストラクタ
	~FPSCamera();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	//プレイヤーから情報をもらってFPS視点を動かす
	//引数:transform プレイヤーのTransform
	// 引数:sensitivity カメラの感度
	void SetFpsCamera(Transform &transform, float sensitivity);

	//FPS視点のオンオフ
	void SetIsPlay(bool state) { isPlay = state; }

	//マウスの移動量を取得
	const int GetMouseDeltaX()const { return DeltaX; }
	const int GetMouseDeltaY()const { return DeltaY; }

	XMFLOAT3 currentMousePos;

	//
	int DeltaX;
	int DeltaY;

	//
	int centerX;
	int centerY;

	//
	bool isPlay;
	bool isCursorHidden = false;
};

