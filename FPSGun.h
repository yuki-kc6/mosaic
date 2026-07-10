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

	//銃を撃ったときのエフェクトを出す
    void BangEffect();

private:
	XMFLOAT3 GetMazzlePosition();//銃口の位置を取得する関数
	int hEffect_;//エフェクトのハンドル
	int hModel_;//モデルのハンドル
    int hSound_;//銃声のハンドル
	EmitterData effectData_;//エフェクトのデータ
}; 

