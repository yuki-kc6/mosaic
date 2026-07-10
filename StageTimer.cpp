#include "StageTimer.h"
#include <chrono>
#include "Engine/SceneManager.h"
#include "Engine/Text.h"
#include "Engine/Image.h"

namespace
{
	constexpr float TIMER_LIMIT = 30.0f;         // 制限時間（秒）

	constexpr float TIMER_GAUGE_WIDTH = 550.0f;   // タイマーゲージ画像の横幅
	constexpr float TIMER_GAUGE_HEIGHT = 64.0f;   // タイマーゲージ画像の高さ

	constexpr float TIMER_BAR_Y = 900.0f;         // タイマーゲージのY座標
	constexpr float TIMER_OUTLINE_X = 600.0f;     // タイマー枠のX座標
	constexpr float TIMER_OUTLINE_Y = 900.0f;     // タイマー枠のY座標

	constexpr float TIMER_BAR_SCALE = 1.5f;       // タイマー画像の拡大率

	constexpr float TIMER_BAR_BASE_X = 800.0f;    // ゲージ左端の位置計算に使用する基準X座標
	constexpr float TIMER_BAR_OFFSET_X = 430.0f;  // ゲージ位置を補正するX方向オフセット


}

StageTimer::StageTimer(GameObject* parent)
	:GameObject(parent,"StageTimer"),limitTime_(0),remainTime_(0),timeGauge_(0), hTimerPic_(-1), hOutLinePic_(-1), isTimeOver_(false), isStart_(false)
{
}

StageTimer::~StageTimer()
{
}

void StageTimer::Initialize()
{
	isStart_ = true;

	hTimerPic_ = Image::Load("TimerBar.png");
	assert(hTimerPic_>=0);

	hOutLinePic_ = Image::Load("TimerOutLine.png");
	assert(hOutLinePic_ >= 0);

	startTime_ = std::chrono::steady_clock::now(); // 計測開始した時間
	limitTime_ = TIMER_LIMIT;
	remainTime_ = limitTime_;
}

void StageTimer::Update()
{
	//スタートしているなら進める
	if (isStart_)
	{
		//現在時刻を取得
		auto now = std::chrono::steady_clock::now();

		//スタートからどれだけ過ぎたか計算
		float elapsed = std::chrono::duration<float>(now - startTime_).count();

		//残り時間を出す
		remainTime_ = limitTime_ - elapsed;

		//0より小さくなったらタイムオーバー
		if (remainTime_ <= 0.0f)
		{
			isTimeOver_ = true;
		}

		//残り時間の割合を計算する
		timeGauge_ = remainTime_ / limitTime_;
	}


}

void StageTimer::Draw()
{
	//現在の表示割合を計算
	float currentWidth = TIMER_GAUGE_WIDTH * timeGauge_;

	//タイマーの中身の表示
	Transform bar;
	
	//切り抜きに合わせてx座標を調整する
	float barLeft = -(TIMER_BAR_BASE_X - currentWidth) * TIMER_BAR_SCALE + TIMER_BAR_OFFSET_X;

	bar.position_.x = barLeft;
	bar.position_.y = TIMER_BAR_Y;
	bar.position_.z = 0;
	bar.scale_ = { TIMER_BAR_SCALE,TIMER_BAR_SCALE,TIMER_BAR_SCALE };
	

	Image::SetRect(hTimerPic_, 0, 0, currentWidth , TIMER_GAUGE_HEIGHT); //時間に合わせて右側が切り取られていく
	Image::SetTransform(hTimerPic_,bar);
	Image::Draw(hTimerPic_);

	//タイマーの枠の表示
	Transform barOut;
	barOut.position_ = { TIMER_OUTLINE_X,TIMER_OUTLINE_Y,0 };
	barOut.scale_ = { TIMER_BAR_SCALE,TIMER_BAR_SCALE,TIMER_BAR_SCALE };
	Image::SetTransform(hOutLinePic_, barOut);
	Image::Draw(hOutLinePic_);


}
 
void StageTimer::Release()
{
}

