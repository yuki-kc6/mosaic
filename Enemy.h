#pragma once
#include "PaintObject.h"
#include <queue>

class StageManager;

enum EnemyState
{
	ENEMY_SPAWN,
	ENEMY_MOVE,
	ENEMY_ENTERBUILDING,
	ENEMY_GETOUTBUILDING,
	ENEMY_WAIT,
	ENEMY_PAINTED
};

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

	void MoveNextTile();

	void SerarchRoad();
	void CreateRoute();


	int hEnemyModel_;
	
	int moveSpeed_;

	int goalX;
	int goalZ;

	int startX;
	int startZ;

	int currentX;
	int currentZ;

	int nextX;
	int nextZ;

	int mapW;
	int mapH;
	StageManager* stageManager;
	EnemyState state_;
	EnemyDirection direction_;
	bool isInBuilding;
	bool isGoal;
	int dx[4] = { 1,0,-1,0 };
	int dz[4] = { 0,1,0,-1 };


	std::vector<EnemyDirection> route;
	std::vector<std::vector<std::pair<int, int>>> parent;
	std::queue<std::pair<int,int>> routeQueue;
	std::vector<std::vector<bool>> visited;
	
	bool isSerarchStarted;
	bool isRouteDecided;
	int routeIndex_;

};