#pragma once
#include "Engine/GameObject.h"
#include <vector>

class StageManager;

class NPCManager : public GameObject
{

public:
    //コンストラクタ
    NPCManager(GameObject* parent);

    //デストラクタ
    ~NPCManager();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;
private:
    std::vector<std::vector<int>> enemy;
	StageManager* stageManager;
    bool isSpawned;
};
