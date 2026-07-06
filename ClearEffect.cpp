#include "ClearEffect.h"
#include "Engine/Camera.h"

ClearEffect::ClearEffect(GameObject* parent)
	: GameObject(parent, "ClearEffect"), hModel_(-1), isEffectActive(false)
{
}

ClearEffect::~ClearEffect()
{
}

void ClearEffect::Initialize()
{
    

}

void ClearEffect::Update()
{

}

void ClearEffect::Draw()
{
	if (isEffectActive) {
        XMFLOAT3 camPos = Camera::GetPosition();
        XMFLOAT3 camTarget = Camera::GetTarget();

        XMVECTOR vForward = XMVector3Normalize(
            XMLoadFloat3(&camTarget) - XMLoadFloat3(&camPos)
        );

        XMFLOAT3 effectPos;
        XMStoreFloat3(&effectPos, XMLoadFloat3(&camPos) + vForward * 5.0f);

        EmitterData data;
        data.textureFileName = "star.png";
        data.position = effectPos;
        data.positionRnd = XMFLOAT3(30.0f, 30.0f, 30.0f);  // 画面全体に広がる
        data.direction = XMFLOAT3(0, 0, 0);
        data.directionRnd = XMFLOAT3(10, 10, 10);
        data.speed = 3.0f;
		data.accel = 1.0f;
        data.lifeTime = 10;
        data.number = 3;
        data.delay = 30;
        data.size = XMFLOAT2(2.0f, 2.0f);
        data.isBillBoard = true;

        VFX::Start(data);
	}

}

void ClearEffect::Release()
{
}

void ClearEffect::EffectEnd()
{
    if (hEffect_)
    {
        VFX::End(hEffect_);
    }
}
