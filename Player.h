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
	XMFLOAT3 baseMousePos;
	XMFLOAT3 currentMousePos;

	XMFLOAT3 mousePos;
	XMFLOAT3 PrevMousePos;

	float camTarY;
	float gravity;
	RenderTexture* paintMap;
	PaintObject* paintObj;
	int centerX;
	int centerY;


};


