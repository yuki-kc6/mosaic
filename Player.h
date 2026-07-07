#pragma once
#include "Engine/GameObject.h"
#include "Engine/Model.h"
#include "FPSCamera.h"

class RenderTexture;
class PaintObject;
class FPSgun;

class Player:public GameObject
{
public:
	//コンストラクタ
	Player(GameObject* parent);
	//デストラクタ
	~Player();
	//初期化
	void Initialize() override;
	//更新
	void Update() override;
	//描画
	void Draw() override;
	//解放
	void Release() override;

	//プレイ状態か決める
	void SetIsPlay(bool state) { isPlay_ = state; }
private:
	//地面にいるか判定する
	void OnGround();
	//ペイントオブジェクトにレイキャストする
	bool RayCastToPaintObjects(RayCastData &data);
	//建物との当たり判定
	void OnCollision(GameObject* pTarget, HitResult result) override;

	int hCrossHair_;//クロスヘア用のハンドル

	FPSCamera* fpsCamera;//fps視点にするためのカメラ
	FPSgun* fpsGun;//FPS視点でひょうじする武器

	float cameraSensitivity_;//カメラにわたす用の感度

	float moveSpeed_;//移動速度
	
	//クロスヘア用の画面中央座標
	float centerX_;
	float centerY_;

	int gunSoundID_;

	bool isPlay_;//プレイ中かのフラグ

};


