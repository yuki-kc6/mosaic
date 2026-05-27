#pragma once
#include "Engine/GameObject.h"
#include <vector>
#include <iostream>
class StageManager : public GameObject
{

public:
    //コンストラクタ
    StageManager(GameObject* parent);

    //デストラクタ
    ~StageManager();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

     //描画
    void Draw() override;

    //開放
    void Release() override;

    int GetMap(int z, int x);
	int GetMapW() { return maps[0].size(); }
	int GetMapH() { return maps.size(); }

    const std::vector<std::pair<int, int>>& GetBuildingList() const
    {
        return buildingList;
    }

private:
    std::vector<std::vector<int>> maps;
	std::vector<std::pair<int, int>> buildingList;
    int a = 0;
};
