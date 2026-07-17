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
    //エフェクトの有効化・無効化
	void SetEffectActive(bool active) { isEffectActive = active; }
    //エフェクトの終了
	void EffectEnd();
private:
    int hClearPic_;//クリアの文字用のハンドル
	EmitterData effectData_;//エフェクトのデータ
	int hEffect_;//エフェクトのハンドル
	bool isEffectActive;//エフェクトが有効かどうか
    bool isClear;//クリアしたかどうか
    int picAlpha_;//クリアの画像のアルファ値

};
