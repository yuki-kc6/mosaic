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
	int hTitlePic_;

	XMFLOAT3 currentPos;
	XMFLOAT3 currentLook;

	XMFLOAT3 targetPos;
	XMFLOAT3 targetLook;

	XMFLOAT3 camPos;
	XMFLOAT3 camTargetPos;
	XMVECTOR vCamPos;
	XMVECTOR vCamTargetPos;

	float currentAngleY = 0.0f;  // 現在の角度
	float targetAngleY = 180.0f; // 目標の角度

	bool isStart;
};