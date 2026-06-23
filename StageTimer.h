#pragma once
#include "Engine/GameObject.h"
#include <chrono>


class Text;

class StageTimer : public GameObject
{

public:
    //コンストラクタ
    StageTimer(GameObject* parent);

    //デストラクタ
    ~StageTimer();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

    bool GetIsTimeOver() { return isTimeOver; }
private:

    float limitTime;//制限時間
    float remainTime;//残り時間

    std::chrono::steady_clock::time_point  startTime;//開始時刻

    float timeGauge;

    int hTimerPic_;
    int hOutLinePic_;

    bool isTimeOver;

};