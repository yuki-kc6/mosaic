#pragma once
#include "PaintObject.h"
#include <queue>
#include <vector>

class StageManager;

//敵の状態
enum NPCState
{
	NPC_START,
	NPC_MOVE,
	NPC_WAIT,
	NPC_PAINTED
};

//敵の移動する方向
enum NPCDirection
{
	NPC_UP,
	NPC_RIGHT,
	NPC_DOWN,
	NPC_LEFT
};

class TownNPC :public PaintObject
{
public:
	TownNPC(GameObject* parent);
	~TownNPC();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	//	void OnCollision(GameObject* pTarget) override;
private:
	//スポーンしたときの初期化
	void ResetRouteSearch();

	void MoveRoute();

	void SearchRoad();//ゴールまでBFS探索する関数
	void CreateRoute();//最短経路を復元する
	void SetTargetPos();//次の目標地点の3D座標をセットする関数
	void SetGoal();//ゴールをビルからランダムに決める関数

	//次の目標地点の3D座標
	XMFLOAT3 targetPos;

	//次の目標地点のマス座標
	int nextTargetX;
	int nextTargetZ;

	//ゴールのマス座標
	int goalX;
	int goalZ;

	//スタートのマス座標
	int startX;
	int startZ;

	//現在のマス座標
	int currentX;
	int currentZ;

	//マップの大きさ
	int mapW;
	int mapH;

	StageManager* stageManager;//ステージマネージャーへのポインタ
	
	//移動する方向
	NPCDirection direction_;

	bool isGoal;
	//BFS探索のための方向配列
	int dx[4] = { 1,0,-1,0 };
	int dz[4] = { 0,1,0,-1 };


	std::vector<std::pair<int, int>> path;//最短経路を復元するための
	std::vector<std::vector<std::pair<int, int>>> parent;//BFS探索のための親ノードを保存する配列
	std::queue<std::pair<int, int>> routeQueue;//BFS探索のためのキュー
	std::vector<std::vector<bool>> visited;//BFS探索のための訪問済み配列

	bool isSerarchStarted = false;//BFS探索が始まったかどうか
	bool isRouteDecided = false;//ルートが決まったかどうか
	int routeIndex_ = 0;//現在のルートのどこにいるか

	bool isInitialized_ = false;

protected:
	//状態
	NPCState state_;
	bool UpdateMove();
	bool UpdateStart();
	bool UpdateWait();
	bool UpdatePainted();

	int hEnemyModel_;

	//移動速度
	float moveSpeed_;

};