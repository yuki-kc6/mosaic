#include "TimeOverEffect.h"
#include "Engine/Image.h"
#include "Engine/Direct3D.h"

namespace
{
	constexpr float GRAY_ALPHA_MAX = 150.0f;//グレーアウト用の画像の最大アルファ値
	constexpr int TIMEOVER_ALPHA_MAX = 255;//クリア画像の透明度の最大値
	constexpr int TIMEOVER_ALPHA_SPEED = 5;//クリア画像の透明度が足される数
}




TimeOverEffect::TimeOverEffect(GameObject* parent)
	:GameObject(parent, "TimeOverEffect"), isTimeOver_(false), hGrayPic_(-1), grayAlpha_(0),hTimeOver_(-1),timeOverAlpha_(0)
{
}

TimeOverEffect::~TimeOverEffect()
{
}

void TimeOverEffect::Initialize()
{
	hGrayPic_ = Image::Load("GrayOut.png");
	assert(hGrayPic_ >= 0);
	hTimeOver_ = Image::Load("GameOver.png");
	assert(hTimeOver_ >= 0);
	
}

void TimeOverEffect::Update()
{
	//時間切れになったらグレーアウト用の画像のアルファ値を増やす
	if (isTimeOver_) {
		if (grayAlpha_ < GRAY_ALPHA_MAX)
		{
			grayAlpha_++;
		}

		if (timeOverAlpha_< TIMEOVER_ALPHA_MAX)
		{
			timeOverAlpha_ += TIMEOVER_ALPHA_SPEED;
		}
		else
		{
			timeOverAlpha_ = TIMEOVER_ALPHA_MAX;
		}

	}
}

void TimeOverEffect::Draw()
{
	//グレーアウト用の画像の描画
	Transform gray;
	gray.position_ = { 0,0,0 };
	gray.scale_ = { (float)Direct3D::screenWidth_,(float)Direct3D::screenHeight_,1 };

	Transform timeover;
	timeover.matTranslate_=XMMatrixTranslation((float)(Direct3D::screenWidth_ / 2),(float)(Direct3D::screenHeight_ / 2), 0 );//画面中央

	Image::SetTransform(hGrayPic_, gray);
	Image::SetTransform(hTimeOver_, timeover);

	//タイムオーバーになったら表示
	if (isTimeOver_)
	{
		Image::SetAlpha(hGrayPic_, grayAlpha_);
		Image::Draw(hGrayPic_);

		Image::SetAlpha(hTimeOver_, timeOverAlpha_);
		Image::Draw(hTimeOver_);
	}

	
	
	



}

void TimeOverEffect::Release()
{
}
