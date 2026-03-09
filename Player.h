#pragma once
#include "Engine/GameObject.h"
#include "Engine/Model.h"

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
	void FPSCamera();
	void OnGround();
	void RayCastToPaintObjects(RayCastData &data);
	void PlayerMove();
	int hModel_;
	XMFLOAT3 currentMousePos;

	XMFLOAT3 camPos;
	XMFLOAT3 camTarget;

	float camTarY;
	float gravity;
	PaintObject* paintObj;
	int centerX;
	int centerY;


};


