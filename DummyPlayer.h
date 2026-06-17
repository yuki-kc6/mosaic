#pragma once
#include "Engine/GameObject.h"

enum PlayerState
{
    IDLE,
    CLEAR
};



class DummyPlayer :
    public GameObject
{
public:
    //コンストラクタ
    DummyPlayer(GameObject* parent);

    //デストラクタ
    ~DummyPlayer();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

    void SetState(PlayerState state);

private:
    PlayerState state_;
};

