#pragma once
#include "Engine/GameObject.h"


class TimeOverEffect : public GameObject
{

public:
    //コンストラクタ
    TimeOverEffect(GameObject* parent);

    //デストラクタ
    ~TimeOverEffect();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

	//時間切れかどうかを設定
	void SetTimeOver(bool state) { isTimeOver_ = state; }

private:
    int hTimeOver_;//タイムオーバー画像用ハンドル
	int hGrayPic_;//グレーアウト用の画像のハンドル
	int grayAlpha_;//グレーアウト用の画像のアルファ値
	bool isTimeOver_;//時間切れかどうかのフラグ
    int timeOverAlpha_;//タイムオーバー画像のアルファ値
};