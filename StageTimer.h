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
private:

    float prevTime;
    float deltaTime;
    float startTime;
    float maxTime;
    float currentTime;
    float elapsed;
    std::chrono::system_clock::time_point  start, end;//時間を測る
    Text* timer;
    int hTimerPic_;
    int hOutLinePic_;

};