#pragma once
#include "Engine/GameObject.h"
#include "Engine/Model.h"
#include "FPSCamera.h"

class RenderTexture;
class PaintObject;

class Player:public GameObject
{
public:
	Player(GameObject* parent);
	~Player();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
private:
	void OnGround();
	void RayCastToPaintObjects(RayCastData &data);
	void PlayerCamera();
	void PlayerMove();
	int hModel_;

	FPSCamera* fpsCamera;

	float yaw_;
	float pitch_;
	float sensitivity_;

	XMFLOAT3 currentMousePos;
	XMFLOAT3 mousePos;
	XMFLOAT3 PrevMousePos;

	
	XMFLOAT3 camPos;
	XMFLOAT3 camTarget;

	float camTarY;
	float gravity;
	PaintObject* paintObj;
	int centerX;
	int centerY;


};


