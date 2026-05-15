#pragma once
#include "PaintObject.h"

class StageManager;

enum EnemyState
{
	ENEMY_MOVE,
	ENEMY_ENTERBUILDING,
	ENEMY_ESCAPE,
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
	void Move();
	std::vector<std::vector<int>> maps;
	int goalX;
	int goalZ;

	int currentX;
	int currentZ;

	int nextX;
	int nextZ;

	int mapW;
	int mapH;
	StageManager* stageManager;
	EnemyState state_;
};