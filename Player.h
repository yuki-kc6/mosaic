#pragma once
#include "Engine/GameObject.h"

struct Mouse
{
	int x=0;
	int y=0;
};


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
	int hModel_;
	XMFLOAT3 mousePos;
	XMFLOAT3 PrevMousePos;
	float camTarY;
	float gravity;
};

