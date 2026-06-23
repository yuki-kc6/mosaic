#pragma once
#include <DirectXMath.h>
#include "Engine/Transform.h"
#include "Engine/GameObject.h"

using namespace DirectX;

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

	void SetFpsCamera(Transform &cam, float sensitivity);

	void SetIsPlay(bool state) { isPlay = state; }

	const int GetMouseDeltaX()const { return DeltaX; }
	const int GetMouseDeltaY()const { return DeltaY; }

private:
	XMFLOAT3 currentMousePos;

	int DeltaX;
	int DeltaY;

	int centerX;
	int centerY;

	bool isPlay;
};

