#pragma once
#include "Engine/GameObject.h"


class GrayOut : public GameObject
{

public:
    //コンストラクタ
    GrayOut(GameObject* parent);

    //デストラクタ
    ~GrayOut();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

	void SetTimeOver(bool state) { isTimeOver = state; }

private:
	int hGrayPic_;
	int grayAlpha;
    bool isTimeOver;

};