#include "PlayScene.h"
#include "Ground.h"
#include "Player.h"
#include "MosaicPrinter.h"
#include "Engine/SceneManager.h"
#include "StageTimer.h"
#include "Building.h"
#include "FPSgun.h"
#include "StageManager.h"
#include "DummyPlayer.h"
#include "NPCManager.h"
#include "Engine/Audio.h"
#include "Engine/Camera.h"
#include "GrayOut.h"
#include "ClearEffect.h"


namespace
{
	constexpr float END_CAMERA_DISTANCE = 30.0f;//終了時のカメラの距離
	constexpr float END_CAMERA_HEIGHT = 10.0f;//終了時のカメラの高さ
	constexpr float END_CAMERA_TARGET_HEIGHT = 3.0f;//終了時のカメラのターゲット高さ

	constexpr float END_CAMERA_MOVE_LERP = 0.03f;//終了時のカメラの位置の補間率
	constexpr float END_CAMERA_TARGET_LERP = 0.01f;//終了時のカメラのターゲットの補間率

	constexpr int   TITLE_CHANGE_FRAME = 180;//タイトルへ戻るまでのフレーム数
}


//コンストラクタ
PlayScene::PlayScene(GameObject * parent)
	: GameObject(parent, "PlayScene"),isEndCameraStarted_(false), isGameOver_(false),isMissionClear_(false),endFrame_(0),titleChangeFrame_(0)
	,endCameraPos_(0,0,0),endCameraTarget_(0,0,0)
{
}

//初期化
void PlayScene::Initialize()
{
	//モザイク塗るための初期化
	MosaicPrinter::Initialize();
	
	//各種オブジェクトの生成
	Instantiate<Ground>(this);	
	Instantiate<StageManager>(this);
	Instantiate<NPCManager>(this);
	Instantiate<DummyPlayer>(this);
	Instantiate<StageTimer>(this);
	Instantiate<Player>(this);
	Instantiate<GrayOut>(this);
	Instantiate<ClearEffect>(this);

	this->PushSensitive();

	
}

//更新
void PlayScene::Update()
{
	//クリアとタイムオーバーを監視
	if (this->CheckMissionClear() || this->CheckTimeOver())
	{
		//まだcameraがスタートしていないなら初期化
		if (!isEndCameraStarted_) {
			StartEndCamera();
			isEndCameraStarted_ = true;
		}
		UpdateEndCamera();//カメラの更新
		
	}


}

//描画
void PlayScene::Draw()
{
	

}

//開放
void PlayScene::Release()
{
	MosaicPrinter::Release();
	Audio::Release();
}

void PlayScene::PushSensitive()
{
	//センシティブなペイントオブジェクトをリストに入れる
    for (PaintObject* pObj : PaintObject::GetPaintObjectList()) {
        // 描画されていない、または死んでいるオブジェクトはスキップ
		if (!pObj->IsVisibled() || pObj->IsDead()) continue;
		if (!pObj->IsSensitive())continue;
		sensitiveList_.push_back(pObj);
    }

}

bool PlayScene::CheckMissionClear()
{
	//センシティブリストがすべて塗られたか判定する
	bool isClear = false;
    PaintObject* closestObj = nullptr;
    for (PaintObject* pObj : sensitiveList_) {
		if (!pObj->IsAllPainted())
		{ 
			isClear = false;
			break;
		}
		isClear = true;
    }
	return isClear;
}

bool PlayScene::CheckTimeOver()
{
	//タイムオーバーになったか判定する
	bool isTimeOver = false;
	StageTimer* timer = (StageTimer*)FindObject("StageTimer");
	if (timer != nullptr) {
		if (timer->GetIsTimeOver())
		{
			isTimeOver = timer->GetIsTimeOver();
		}
	}
	return isTimeOver;
}

void PlayScene::StartEndCamera()
{
	//結果を入れる
	isGameOver_ = CheckTimeOver();
	isMissionClear_ = CheckMissionClear();

	//各種オブジェクトを終了画面仕様に更新

	FPSCamera* fpsCamera = (FPSCamera*)FindObject("FPSCamera");
	if (fpsCamera) {
		fpsCamera->SetIsPlay(false);//プレイ中をfalse
	}

	FPSgun* fpsGun = (FPSgun*)FindObject("FPSgun");
	if (fpsGun) {
		fpsGun->KillMe();//消す
	}
	Player* player = (Player*)FindObject("Player");
	if (player) {
		player->Invisible();//クロスヘアを透明にする
		player->SetIsPlay(false);//プレイ中ではない
	}
	DummyPlayer* dummyPlayer = (DummyPlayer*)FindObject("DummyPlayer");
	if (dummyPlayer) {
		dummyPlayer->Visible();//透明化を解除
		if (isMissionClear_)
		{
			//クリア時のモーション
			dummyPlayer->SetState(PlayerState::CLEAR);

		}
	}

	StageTimer* timer = (StageTimer*)FindObject("StageTimer");
	if (timer) {
		timer->SetTimer(false);//タイマーを止める
		timer->Invisible();//タイマーを非表示
	}

	
	GrayOut* grayOut = (GrayOut*)FindObject("GrayOut");
	if (grayOut) {
		grayOut->SetTimeOver(isGameOver_);//ゲームオーバーならグレーアウトさせる
	}

	
	ClearEffect* clearEffect = (ClearEffect*)FindObject("ClearEffect");
	if(clearEffect) {
		clearEffect->SetEffectActive(isMissionClear_);//ミッションクリアならクリアエフェクトを出す
	}

	//プレイヤーの向きを取得
	float rotY = player->GetRotate().y;
	float rad = XMConvertToRadians(rotY);

	//プレイヤーの座標を取得
	XMFLOAT3 playerPos = player->GetPosition();

	//プレイヤーの場所からカメラの目標位置を設定
	endCameraPos_.x = playerPos.x - sinf(rad) * END_CAMERA_DISTANCE;
	endCameraPos_.y = playerPos.y + END_CAMERA_HEIGHT;
	endCameraPos_.z = playerPos.z - cosf(rad) * END_CAMERA_DISTANCE;

	//プレイヤーの場所からカメラの目標とするターゲットを設定
	endCameraTarget_.x = playerPos.x;
	endCameraTarget_.y = playerPos.y + END_CAMERA_TARGET_HEIGHT;
	endCameraTarget_.z = playerPos.z;

	titleChangeFrame_ = TITLE_CHANGE_FRAME;
}

void PlayScene::UpdateEndCamera()
{

	//現在のカメラ座標
	XMFLOAT3 currentCameraPos = Camera::GetPosition();
	XMFLOAT3 currentCameraTarget = Camera::GetTarget();

	//ベクトル化
	XMVECTOR vCurrentCameraPos = XMLoadFloat3(&currentCameraPos);
	XMVECTOR vCurrentCameraTarget = XMLoadFloat3(&currentCameraTarget);

	//終了位置もベクトル化
	XMVECTOR vEndCameraPos = XMLoadFloat3(&endCameraPos_);
	XMVECTOR vEndCameraTarget = XMLoadFloat3(&endCameraTarget_);


	XMVECTOR vCameraPos;
	XMVECTOR vCameraTarget;

	//線形補間でポジションとターゲットを動かす
	vCameraPos = XMVectorLerp(vCurrentCameraPos, vEndCameraPos, END_CAMERA_MOVE_LERP);
	vCameraTarget = XMVectorLerp(vCurrentCameraTarget, vEndCameraTarget, END_CAMERA_TARGET_LERP);

	XMFLOAT3 cameraPos;
	XMFLOAT3 cameraTarget;
	XMStoreFloat3(&cameraPos, vCameraPos);
	XMStoreFloat3(&cameraTarget, vCameraTarget);

	Camera::SetPosition(cameraPos);
	Camera::SetTarget(cameraTarget);

	//終了までのフレームを更新
	endFrame_++;

	//超えたらシーンチェンジ
	if (endFrame_ >= titleChangeFrame_)
	{
		ClearEffect* clearEffect = (ClearEffect*)FindObject("ClearEffect");
		clearEffect->EffectEnd();

		SceneManager* sm = (SceneManager*)FindObject("SceneManager");
		sm->ChangeScene(SCENE_ID_TITLE);
	}

}
