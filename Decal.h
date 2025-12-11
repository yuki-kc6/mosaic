#pragma once
#include "Engine/GameObject.h"

struct DECAL_AXIS {

	XMFLOAT3 front;
	XMFLOAT3 right;
	XMFLOAT3 up;
};


class Decal :public GameObject
{
public:
	Decal(GameObject* parent);
	~Decal();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void SetAxis(const DECAL_AXIS& vec) { this->vec = vec; };
	DECAL_AXIS GetAxis() { return vec; }
private:
	int hModel_;
	DECAL_AXIS vec;
};



