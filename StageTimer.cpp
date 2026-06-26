#include "StageTimer.h"
#include <chrono>
#include "Engine/SceneManager.h"
#include "Engine/Text.h"
#include "Engine/Image.h"

StageTimer::StageTimer(GameObject* parent)
	:limitTime(0),hTimerPic_(-1),isTimeOver(false),isStart(true)
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
	limitTime = 300.0f;
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
	float currentWidth = 550.0f * timeGauge;

	Transform bar;

	bar.position_.x =  -(800.0f - currentWidth)*1.5+430 ;
	bar.position_.y = 900;
	bar.position_.z = 0;
	bar.scale_ = { 1.5,1.5,1.5 };
	
	Image::SetRect(hTimerPic_, 0, 0, currentWidth , 64); //時間に合わせて右側が切り取られていく
	Image::SetTransform(hTimerPic_,bar);
	Image::Draw(hTimerPic_);

	Transform barOut;
	barOut.position_ = { 600,900,0 };
	barOut.scale_ = { 1.5,1.5,1.5 };
	Image::SetTransform(hOutLinePic_, barOut);
	Image::Draw(hOutLinePic_);


}
 
void StageTimer::Release()
{
}

