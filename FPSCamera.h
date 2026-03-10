#pragma once
#include <DirectXMath.h>
#include "Engine/Transform.h"

using namespace DirectX;

class FPSCamera
{
public:
	FPSCamera();
	~FPSCamera();

	void Update();

	void SetFpsCamera(Transform &cam, float sensitivity);

	const int GetMouseDeltaX()const { return DeltaX; }
	const int GetMouseDeltaY()const { return DeltaY; }

private:
	XMFLOAT3 currentMousePos;

	int DeltaX;
	int DeltaY;

	int centerX;
	int centerY;

};

