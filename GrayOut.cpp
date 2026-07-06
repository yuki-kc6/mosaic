#include "GrayOut.h"
#include "Engine/Image.h"
#include "Engine/Direct3D.h"

namespace
{
	constexpr float GRAY_ALPHA_MAX = 150.0f;
}




GrayOut::GrayOut(GameObject* parent)
	:GameObject(parent, "GrayOut"), isTimeOver(false), hGrayPic_(-1), grayAlpha(0)
{
}

GrayOut::~GrayOut()
{
}

void GrayOut::Initialize()
{
	hGrayPic_ = Image::Load("GrayOut.png");
	assert(hGrayPic_ >= 0);
}

void GrayOut::Update()
{
	if (isTimeOver) {
		if (grayAlpha < GRAY_ALPHA_MAX)
		{
			grayAlpha++;
		}
	}
}

void GrayOut::Draw()
{
	Transform gray;
	gray.position_ = { 0,0,0 };
	gray.scale_ = { (float)Direct3D::screenWidth_,(float)Direct3D::screenHeight_,1 };

	if (isTimeOver)
	{
		Image::SetTransform(hGrayPic_, gray);
		Image::SetAlpha(hGrayPic_, grayAlpha);
		Image::Draw(hGrayPic_);

	}
}

void GrayOut::Release()
{
}
