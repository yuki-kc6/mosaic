#pragma once
#include "Engine/GameObject.h"


class RenderTexture;

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
	int hModel_;
	XMFLOAT3 baseMousePos;
	XMFLOAT3 currentMousePos;

	XMFLOAT3 mousePos;
	XMFLOAT3 PrevMousePos;

	float camTarY;
	float gravity;
	RenderTexture* paintMap;
	float ScreenWIDTH = 800;
	float ScreenHEIGHT = 600;

};
