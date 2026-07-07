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
	//センシティブオブジェクトをまとめる
	void PushSensitive();
	//センシティブオブジェクトが全て塗られたか判定する
	bool CheckMissionClear();
	//タイムオーバーになったか判定する
	bool CheckTimeOver();

	//ゲーム終了時のカメラの処理の初期化
	void StartEndCamera();
	//ゲーム終了時のカメラの更新
	void UpdateEndCamera();



	XMFLOAT3 endCameraPos_;
	XMFLOAT3 endCameraTarget_;
	
	bool isEndCameraStarted_;

	bool isGameOver_;
	bool isMissionClear_;

	int endFrame_;
	int titleChangeFrame_;
	std::list<PaintObject*> sensitiveList_;
};