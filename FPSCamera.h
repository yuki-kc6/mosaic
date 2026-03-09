#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class FPSCamera
{
public:
	FPSCamera();
	~FPSCamera();

	void Update();

	const int GetMouseDelatX()const { return DeltaX; }
	const int GetMouseDelatY()const { return DeltaY; }

private:
	XMFLOAT3 currentMousePos;

	int DeltaX;
	int DeltaY;

	int centerX;
	int centerY;

};

