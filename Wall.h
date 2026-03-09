#pragma once
#include "PaintObject.h"

//◆◆◆を管理するクラス
class Wall : public PaintObject
{

public:
    //コンストラクタ
    Wall(GameObject* parent);

    //デストラクタ
    ~Wall();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;
private:

};