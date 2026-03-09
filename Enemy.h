#pragma once
#include "PaintObject.h"

class Enemy :public PaintObject
{
public:
	Enemy(GameObject* parent);
	~Enemy();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;
private:
};