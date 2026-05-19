#pragma once
#include "PaintObject.h"

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
	ENEMY_DOWN,
	ENEMY_LEFT,
	ENEMY_RIGHT
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
	void MoveNextTile();

	void DecideNextTile();

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
};