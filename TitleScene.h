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

	int goPic_;

	XMFLOAT3 currentPos;
	XMFLOAT3 targetPos;

	XMFLOAT3 currentLook;


	float currentAngleY = 0.0f;
	const float rotateSpeed = 0.5f;  // 1フレームに何度回るか
	const float targetAngle = 90.0f; // 真後ろ



	bool isStart;
};