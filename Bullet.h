#pragma once
#include "Engine/GameObject.h"
#include "Engine/Model.h"

struct Vector2
{
	float x;
	float y;
};

struct BoxDecal {
	XMVECTOR pos;//中心点
	float width;//幅
	float height;//高さ
	float depth;//奥行
	bool isEnabled;//有効フラグ
};

class Bullet :public GameObject
{
public:
	Bullet(GameObject* parent);
	~Bullet();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;
	//void SetMove(XMVECTOR v) { move_ = v; }
private:
	int hModel_;
	int tModel;
	int life_;
	XMVECTOR move_;
	GameObject* pTarget_;
	RayCastData data;
};



