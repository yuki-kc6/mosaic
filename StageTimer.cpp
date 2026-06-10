#include "StageTimer.h"
#include <chrono>
#include "Engine/SceneManager.h"
#include "Engine/Text.h"


StageTimer::StageTimer(GameObject* parent)
	:prevTime(0),startTime(0),maxTime(0),timer(nullptr)
{
}

StageTimer::~StageTimer()
{
}

void StageTimer::Initialize()
{
	
	start = std::chrono::system_clock::now(); // Œv‘ªŠJŽn‚µ‚½ŽžŠÔ
	maxTime = 60.0f;
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
	char time = (char)(maxTime - elapsed);
	//timer->Draw(0, 0, "ABSHODJEFDOJ")
}
 
void StageTimer::Release()
{
}
