#pragma once
#include "Engine/GameObject.h"
#include <vector>
class EnemyManager : public GameObject
{

public:
    //コンストラクタ
    EnemyManager(GameObject* parent);

    //デストラクタ
    ~EnemyManager();

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
    std::vector<std::vector<int>> maps;
};
