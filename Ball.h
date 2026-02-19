#pragma once
#include "PaintObject.h"
class Ball :
    public PaintObject
{
public:
    //コンストラクタ
    Ball(GameObject* parent);

    //デストラクタ
    ~Ball();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;
private:
    int hModel_;    //モデル番号

};

