#pragma once
#include "Engine/GameObject.h"
#include <vector>
#include <iostream>


enum MapChip
{
    MAP_SIDEWALK = 0,
    MAP_BUILDING = 1,
    MAP_ROAD = 2,
    MAP_CROSSWALK = 3
};

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

    //座標ごとのMapを返す
    int GetMap(int z, int x);

    //マップの大きさを変えす
	int GetMapW() { return (int)maps_[0].size(); }
	int GetMapH() { return (int)maps_.size(); }

    //マップ上での１マスの幅を返す
    float GetGridSize() { return gridSize_; }

    //ビルディングリストを返す
    const std::vector<std::pair<int, int>>& GetBuildingList() const
    {
        return buildingList_;
    }

private:
    void BuildStage();//ステージに配置する
    std::vector<std::vector<int>> maps_;//mapの中身
	std::vector<std::pair<int, int>> buildingList_;//ビルの座標を入れる
    float gridSize_;//１マスの大きさ
};
