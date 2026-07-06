#pragma once
#include "Engine/GameObject.h"
#include "Engine/VFX.h"
class ClearEffect :
    public GameObject
{
public:
    //コンストラクタ
    ClearEffect(GameObject* parent);
    //デストラクタ
    ~ClearEffect();
    //初期化
    void Initialize() override;
    //更新
    void Update() override;
    //描画
    void Draw() override;
    //開放
	void Release() override;

	void SetEffectActive(bool active) { isEffectActive = active; }
private:
    int hModel_;
	EmitterData effectData_;
	bool isEffectActive;

};

