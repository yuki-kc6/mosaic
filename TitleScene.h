#pragma once
#include "Engine/GameObject.h"

enum CameraState
{
	CMAERA_
};

//テストシーンを管理するクラス
class TitleScene : public GameObject
{
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	TitleScene(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
private:
	int hTitleBGM_;//BGM用ハンドル

	XMFLOAT3 cameraCurrentPos_;//カメラの現在位置
	XMFLOAT3 cameraGoalPos_;//カメラの目標位置

	XMFLOAT3 cameraCurrentTarget_;//カメラが今どこを見ているか

	float currentAngleY_;//カメラが今Y座標をどれだけ回転しているか

	bool isStart_;//startしているかのフラグ
};