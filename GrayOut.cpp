#include "GrayOut.h"
#include "Engine/Image.h"
#include "Engine/Direct3D.h"

namespace
{
	constexpr float GRAY_ALPHA_MAX = 150.0f;//グレーアウト用の画像の最大アルファ値
}




GrayOut::GrayOut(GameObject* parent)
	:GameObject(parent, "GrayOut"), isTimeOver_(false), hGrayPic_(-1), grayAlpha_(0)
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
	//時間切れになったらグレーアウト用の画像のアルファ値を増やす
	if (isTimeOver_) {
		if (grayAlpha_ < GRAY_ALPHA_MAX)
		{
			grayAlpha_++;
		}
	}
}

void GrayOut::Draw()
{
	//グレーアウト用の画像の描画
	Transform gray;
	gray.position_ = { 0,0,0 };
	gray.scale_ = { (float)Direct3D::screenWidth_,(float)Direct3D::screenHeight_,1 };

	//時間切れで表示
	if (isTimeOver_)
	{
		Image::SetTransform(hGrayPic_, gray);
		Image::SetAlpha(hGrayPic_, grayAlpha_);
		Image::Draw(hGrayPic_);

	}
}

void GrayOut::Release()
{
}
