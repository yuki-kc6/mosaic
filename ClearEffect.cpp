#include "ClearEffect.h"

ClearEffect::ClearEffect(GameObject* parent)
	: GameObject(parent, "ClearEffect"), hModel_(-1)
{
}

ClearEffect::~ClearEffect()
{
}

void ClearEffect::Initialize()
{
	effectData_.textureFileName = "cloudA.png";
	effectData_.speed = 3.0f;
	effectData_.accel = -0.05f;      // 少し減速
	effectData_.direction = { 0,1,0 }; // 上方向
	effectData_.directionRnd = { 2,2,2 };
	effectData_.lifeTime = 60;

}

void ClearEffect::Update()
{
}

void ClearEffect::Draw()
{
	if (isEffectActive) {
		VFX::Start(effectData_);
	}

}

void ClearEffect::Release()
{
}
