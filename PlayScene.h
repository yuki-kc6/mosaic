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

	void MissionClearStart();
	void TimeOverStart();

	int hPlayBGM_;//BGM用ハンドル
	int hClearFanfarel_;//クリア時サウンド用ハンドル
	int hTimeOverSound_;//タイムオーバー時サウンド用ハンドル

	XMFLOAT3 endCameraPos_;//終了時のカメラの位置座標
	XMFLOAT3 endCameraTarget_;//終了時のカメラのターゲット座標
	
	bool isEndCameraStarted_;//終了カメラが始まったかのフラグ

	bool isGameOver_;//ゲームオーバーのなったかのフラグ
	bool isMissionClear_;//クリアしたかのフラグ

	int endFrame_;//終了までのフレーム
	int titleChangeFrame_;//タイトルにいくフレーム
	std::list<PaintObject*> sensitiveList_;//センシティブなオブジェクトを入れるリスト
};