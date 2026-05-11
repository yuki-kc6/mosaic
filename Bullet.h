#pragma once
#include "Engine/GameObject.h"
#include "Engine/Model.h"


class Bullet :public GameObject
{
public:
	Bullet(GameObject* parent);
	~Bullet();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	//void OnCollision(GameObject* pTarget) override;
	//void SetMove(XMVECTOR v) { move_ = v; }
private:
	int hModel_;
	int tModel;
	int life_;
	XMVECTOR move_;
	GameObject* pTarget_;
	RayCastData data;
};



