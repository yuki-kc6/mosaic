#include "StageTimer.h"
#include <chrono>
#include "Engine/SceneManager.h"
#include "Engine/Text.h"
#include "Engine/Image.h"

namespace
{
	constexpr float TIMER_LIMIT = 10.0f;

	constexpr float TIMER_GAUGE_WIDTH = 550.0f;
	constexpr float TIMER_GAUGE_HEIGHT = 64.0f;

	constexpr float TIMER_BAR_Y = 900.0f;
	constexpr float TIMER_OUTLINE_X = 600.0f;
	constexpr float TIMER_OUTLINE_Y = 900.0f;

	constexpr float TIMER_BAR_SCALE = 1.5f;

	constexpr float TIMER_BASE_X = 800.0f;
	constexpr float TIMER_BAR_OFFSET_X = 430.0f;


}

StageTimer::StageTimer(GameObject* parent)
	:GameObject(parent,"StageTimer"),limitTime(0), hTimerPic_(-1), isTimeOver(false), isStart(true)
{
}

StageTimer::~StageTimer()
{
}

void StageTimer::Initialize()
{
	hTimerPic_ = Image::Load("TimerBar.png");
	assert(hTimerPic_>=0);

	hOutLinePic_ = Image::Load("TimerOutLine.png");
	assert(hOutLinePic_ >= 0);

	startTime = std::chrono::steady_clock::now(); // 計測開始した時間
	limitTime = TIMER_LIMIT;
	remainTime = limitTime;
}

void StageTimer::Update()
{
	if (isStart)
	{
		auto now = std::chrono::steady_clock::now();

		float elapsed = std::chrono::duration<float>(now - startTime).count();

		remainTime = limitTime - elapsed;

		if (remainTime <= 0.0f)
		{
			isTimeOver = true;
		}

		timeGauge = remainTime / limitTime;
	}


}

void StageTimer::Draw()
{
	float currentWidth = TIMER_GAUGE_WIDTH * timeGauge;

	Transform bar;

	float barLeft = -(TIMER_BASE_X - currentWidth) * TIMER_BAR_SCALE + TIMER_BAR_OFFSET_X;

	bar.position_.x = barLeft;
	bar.position_.y = TIMER_BAR_Y;
	bar.position_.z = 0;
	bar.scale_ = { TIMER_BAR_SCALE,TIMER_BAR_SCALE,TIMER_BAR_SCALE };
	
	Image::SetRect(hTimerPic_, 0, 0, currentWidth , TIMER_GAUGE_HEIGHT); //時間に合わせて右側が切り取られていく
	Image::SetTransform(hTimerPic_,bar);
	Image::Draw(hTimerPic_);

	Transform barOut;
	barOut.position_ = { TIMER_OUTLINE_X,TIMER_OUTLINE_Y,0 };
	barOut.scale_ = { TIMER_BAR_SCALE,TIMER_BAR_SCALE,TIMER_BAR_SCALE };
	Image::SetTransform(hOutLinePic_, barOut);
	Image::Draw(hOutLinePic_);


}
 
void StageTimer::Release()
{
}

