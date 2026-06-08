#pragma once
#include "Engine/GameObject.h"
#include "Engine/VFX.h"
    class FPSgun : public GameObject
{

public:
    //コンストラクタ
    FPSgun(GameObject* parent);

    //デストラクタ
    ~FPSgun();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

    void BangEffect();

private:
    XMFLOAT3 GetMazzlePosition();

    int hModel_;
    EmitterData effectData_;
}; 

