#include "ClearEffect.h"
#include "Engine/Camera.h"

namespace
{
	constexpr float EFFECT_POSITION_RND = 30.0f;//エフェクトの位置のランダム範囲
	constexpr float EFFECT_DIRECTION_RND = 10.0f;//エフェクトの方向のランダム範囲
	constexpr float EFFECT_SPEED = 1.0f;//エフェクトの速度
	constexpr float EFFECT_ACCEL = 1.0f;//エフェクトの加速度
	constexpr float EFFECT_LIFETIME = 10.0f;//エフェクトの寿命
	constexpr float EFFECT_NUMBER = 10.0f;//エフェクトの数
	constexpr float EFFECT_DELAY = 30.0f;//エフェクトの遅延
	constexpr float EFFECT_SIZE = 2.0f;//エフェクトのサイズ
}

ClearEffect::ClearEffect(GameObject* parent)
	: GameObject(parent, "ClearEffect"), hEffect_(-1), isEffectActive(false)
{
}

ClearEffect::~ClearEffect()
{
    EffectEnd();
}

void ClearEffect::Initialize()
{

	effectData_.textureFileName = "star.png";//星の画像を使用
    effectData_.positionRnd = XMFLOAT3(EFFECT_POSITION_RND, EFFECT_POSITION_RND, EFFECT_POSITION_RND);  // 画面全体に広がるように
    effectData_.direction = XMFLOAT3(0, 0, 0);
	effectData_.directionRnd = XMFLOAT3(EFFECT_DIRECTION_RND, EFFECT_DIRECTION_RND, EFFECT_DIRECTION_RND);//方向の誤差を大きくしてランダムに飛ばす
    effectData_.speed = EFFECT_SPEED;//速度
    effectData_.accel = EFFECT_ACCEL;//加速度
    effectData_.lifeTime = EFFECT_LIFETIME;//寿命
    effectData_.number = EFFECT_NUMBER;//エフェクトの数
    effectData_.delay = EFFECT_DELAY;//エフェクトの遅延
    effectData_.size = XMFLOAT2(EFFECT_SIZE, EFFECT_SIZE);
    effectData_.isBillBoard = true;
}

void ClearEffect::Update()
{
    if (isEffectActive) {
		// カメラの位置とターゲットを取得
        XMFLOAT3 camPos = Camera::GetPosition();
        XMFLOAT3 camTarget = Camera::GetTarget();

		// カメラの前方向を計算
        XMVECTOR vForward = XMVector3Normalize(
            XMLoadFloat3(&camTarget) - XMLoadFloat3(&camPos)
        );

		// エフェクトの位置をカメラの前方に設定
        XMFLOAT3 effectPos;
        XMStoreFloat3(&effectPos, XMLoadFloat3(&camPos) + vForward * 5.0f);
        effectData_.position = effectPos;

        if(hEffect_ != -1)
		{
			VFX::End(hEffect_);
		}
		// エフェクトを開始
        hEffect_ = VFX::Start(effectData_);
    }
}

void ClearEffect::Draw()
{
	

}

void ClearEffect::Release()
{

}

void ClearEffect::EffectEnd()
{
	//エフェクトを終了
	isEffectActive = false;
    if (hEffect_)
    {
        VFX::End(hEffect_);
    }
}
