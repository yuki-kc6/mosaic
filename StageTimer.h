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

    bool GetIsTimeOver() { return isTimeOver_; }

    void SetTimer(bool start) { isStart_ = start; }
private:

    float limitTime_;//制限時間
    float remainTime_;//残り時間

    std::chrono::steady_clock::time_point  startTime_;//開始時刻

    float timeGauge_;//残り時間の割合

    int hTimerPic_;//タイマーのハンドル
    int hOutLinePic_;//アウトラインのハンドル

    bool isTimeOver_;//タイムオーバーのフラグ

    bool isStart_;//開始しているか

};