#pragma once
#include "Engine/GameObject.h"
#include <list>

class PaintObject;

//テストシーンを管理するクラス
class PlayScene : public GameObject
{
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	PlayScene(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;	
private:
	void PushSensitive();
	bool CheckMissionClear();
	bool CheckTimeOver();

	void TimerOverEffect();

	void StartEndCamera();
	void UpdateEndCamera();



	XMFLOAT3 endCameraPos;
	XMFLOAT3 endCameraTarget;
	
	bool isEndCameraStarted;

	bool isGameOver;
	bool isMissionClear;

	int endFrame = 0;
	const int kTitleChangeFrame = 180; // 3秒
	std::list<PaintObject*> sensitiveList_;
};