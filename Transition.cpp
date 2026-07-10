#include "Transition.h"

namespace Transition
{
	float progress = 0.0f;
	float speed = 0.02f;
	bool playing = false;
	bool fadeOut = false;

	void Transition::Initialize()
	{

	}

	void Update()
	{
		if (!playing)
			return;

		if (fadeOut)
		{
			progress += speed;

			if (progress >= 1.0f)
			{
				progress = 1.0f;
				playing = false;
			}
		}
		else
		{
			progress -= speed;

			if (progress <= 0.0f)
			{
				progress = 0.0f;
				playing = false;
			}
		}
	}

	void Draw()
	{
	}

	void Transition::StartFadeOut()
	{ 
		playing = true;
		fadeOut = true;
		progress = 0.0f;
	}

	void Transition::StartFadeIn()
	{
		playing = true;
		fadeOut = false;
		progress = 1.0f;
	}

	void Release()
	{
	}



}