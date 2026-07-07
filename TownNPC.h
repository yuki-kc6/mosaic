#pragma once
#include "PaintObject.h"
#include <queue>
#include <vector>

class StageManager;

//敵の状態
enum NPCState
{
	NPC_START,//スターと
	NPC_MOVE,//動く
	NPC_WAIT,//待機
	NPC_PAINTED//塗られた
};

//敵の移動する方向
enum NPCDirection
{
	NPC_UP,//CSV上で上
	NPC_RIGHT,//右
	NPC_DOWN,//下
	NPC_LEFT//左
};

class TownNPC :public PaintObject
{
public:
	//コンストラクタ
	TownNPC(GameObject* parent);
	//デストラクタ
	~TownNPC();
	//初期化
	void Initialize() override;
	//更新
	void Update() override;
	//描画
	void Draw() override;
	//解放
	void Release() override;
private:

	static int hMoedl_;

	//スポーンしたときの初期化
	void ResetRouteSearch();

	//ルートを歩く
	void MoveRoute();

	void SearchRoad();//ゴールまでBFS探索する関数
	void CreateRoute();//最短経路を復元する
	void SetTargetPos(NPCDirection dir);//次の目標地点の3D座標をセットする関数
	void SetGoal();//ゴールをビルからランダムに決める関数

	NPCDirection SetDirection();//NPCの方向を決める関数

	//次の目標地点の3D座標
	XMFLOAT3 targetPos;

	//次の目標地点のマス座標
	float nextTargetX_;
	float nextTargetZ_;

	//ゴールのマス座標
	int goalX_;
	int goalZ_;

	//スタートのマス座標
	int startX_;
	int startZ_;

	//現在のマス座標
	int currentX_;
	int currentZ_;

	//マップの大きさ
	int mapW_;
	int mapH_;

	StageManager* stageManager;//ステージマネージャーへのポインタ
	
	//移動する方向
	NPCDirection direction_;

	bool isGoal_;
	//BFS探索のための方向配列
	int dx[4] = { 1,0,-1,0 };
	int dz[4] = { 0,1,0,-1 };


	std::vector<std::pair<int, int>> path_;//最短経路を復元するための
	std::vector<std::vector<std::pair<int, int>>> parent_;//BFS探索のための親ノードを保存する配列
	std::queue<std::pair<int, int>> routeQueue_;//BFS探索のためのキュー
	std::vector<std::vector<bool>> visited_;//BFS探索のための訪問済み配列

	bool isSerarchStarted_ ;//BFS探索が始まったかどうか
	bool isRouteDecided_ ;//ルートが決まったかどうか
	int routeIndex_;//現在のルートのどこにいるか

protected:
	//状態
	NPCState state_;
	bool UpdateMove();
	bool UpdateStart();
	bool UpdateWait();
	bool UpdatePainted();


	//移動速度
	float moveSpeed_;

	float gridSize_;
};