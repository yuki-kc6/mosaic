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
	//NPCを生成する関数
    void SpawnNPC();
	StageManager* stageManager;//ステージマネージャーのポインタ


	float gridSize;//ステージのグリッドサイズ
	std::vector<std::pair<int, int>> spawnList;//NPCを生成する座標のリスト

};
