#include "TitleHeader.h"
#include "Engine/Image.h"

TitleHeader::TitleHeader(GameObject* parent)
	: GameObject(parent, "TitleHeader"), hPic_(-1)
{
}

TitleHeader::~TitleHeader()
{
}

void TitleHeader::Initialize()
{
	hPic_ = Image::Load("title.png");
	assert(hPic_ >= 0);
	hTitle_ = Image::Load("titleName.png");
}

void TitleHeader::Update()
{
}

void TitleHeader::Draw()
{
	transform_.position_ = { 0,-600,0 };
	transform_.scale_ = { 0.2,0.2,1 };
	Image::SetTransform(hPic_, transform_);
	Image::Draw(hPic_);

	Transform title;
	title.position_ = { 3,500,0 };
	transform_.scale_ = { 1,1,1 };
	Image::SetTransform(hTitle_, title);
	Image::Draw(hTitle_);
}

void TitleHeader::Release()
{
}
