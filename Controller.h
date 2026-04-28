#pragma once
#include "PaintObject.h"

class Controller : public PaintObject
{

public:
    //コンストラクタ
    Controller(GameObject* parent);

    //デストラクタ
    ~Controller();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;
private:
    bool MoveForward();
    bool MoveBack();

    float moveSpeed_;

};