#include "StageTimer.h"
#include <chrono>
#include "Engine/SceneManager.h"
#include "Engine/Text.h"
#include "Engine/Image.h"

StageTimer::StageTimer(GameObject* parent)
	:prevTime(0),startTime(0),maxTime(0),timer(nullptr),hTimerPic_(-1)
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

	start = std::chrono::system_clock::now(); // 計測開始した時間
	maxTime = 600.0f;
	timer = new Text;
	timer->Initialize();
}

void StageTimer::Update()
{
	end = std::chrono::system_clock::now();

	elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
	if (maxTime - elapsed < 0)
	{
		SceneManager* sm = (SceneManager*)FindObject("SceneManager");
		sm->ChangeScene(SCENE_ID_RESULT);
	}

}

void StageTimer::Draw()
{

	Transform bar;
	bar.position_ = { 0,900,0 };
	bar.scale_ = { 1.5,1.5,1.5 };
	//Image::SetRect(hTimerPic_,) 時間に合わせて右側が切り取られていく
	Image::SetTransform(hTimerPic_,bar);
	Image::Draw(hTimerPic_);

	Transform barOut;
	barOut.position_ = { 0,900,0 };
	barOut.scale_ = { 1.5,1.5,1.5 };
	Image::SetTransform(hOutLinePic_, barOut);
	Image::Draw(hOutLinePic_);


}
 
void StageTimer::Release()
{
}
