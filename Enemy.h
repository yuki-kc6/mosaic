#pragma once
#include "PaintObject.h"
#include <queue>
#include <vector>

class StageManager;

//敵の状態
enum EnemyState
{
	ENEMY_SPAWN,
	ENEMY_MOVE,
	ENEMY_SETTARGETPOS,
	ENEMY_WAIT,
	ENEMY_PAINTED
};

//敵の移動する方向
enum EnemyDirection
{
	ENEMY_UP,
	ENEMY_RIGHT,
	ENEMY_DOWN,
	ENEMY_LEFT
};

class Enemy :public PaintObject
{
public:
	Enemy(GameObject* parent);
	~Enemy();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
//	void OnCollision(GameObject* pTarget) override;
private:
	void UpdateMove();

	void MoveRoute();

	void SearchRoad();
	void CreateRoute();
	void SetTargetPos();


	//次の目標地点の3D座標
	XMFLOAT3 targetPos;

	//次の目標地点のマス座標
	int nextTargetX;
	int nextTargetZ;

	int hEnemyModel_;
	
	//移動速度
	int moveSpeed_;

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
	//状態
	EnemyState state_;
	//移動する方向
	EnemyDirection direction_;

	bool isGoal;
	//BFS探索のための方向配列
	int dx[4] = { 1,0,-1,0 };
	int dz[4] = { 0,1,0,-1 };


	std::vector<EnemyDirection> route;//スタートからゴールまでのルートをDirectionで表す
	std::vector<std::vector<std::pair<int, int>>> parent;//BFS探索のための親ノードを保存する配列
	std::queue<std::pair<int, int>> routeQueue;//BFS探索のためのキュー
	std::vector<std::vector<bool>> visited;//BFS探索のための訪問済み配列
	
	bool isSerarchStarted=false;//BFS探索が始まったかどうか
	bool isRouteDecided=false;//ルートが決まったかどうか
	int routeIndex_=0;//現在のルートのどこにいるか

	bool isInitialized_ = false;


};