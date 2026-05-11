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
	void OnCollision(GameObject* pTarget, HitResult result) override;
	int hModel_;
	int hCrossHair_;

	FPSCamera* fpsCamera;

	float moveSpeed_;
	
	float centerX;
	float centerY;

};


