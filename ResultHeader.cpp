#include "ResultHeader.h"
#include "Engine/Image.h"

ResultHeader::ResultHeader(GameObject* parent)
	: GameObject(parent, "Player"), hPic_(-1)
{
}

ResultHeader::~ResultHeader()
{
}

void ResultHeader::Initialize()
{
	hPic_ = Image::Load("result.png");
	assert(hPic_ >= 0);
}

void ResultHeader::Update()
{
}

void ResultHeader::Draw()
{
	//transform_.position_ = { 0, 0, 0 };
	//transform_.rotate_ = { 0, 0, 0 };
	//transform_.scale_ = { 1, 1, 1 };

	//transform_.position_ = { 300, 200, 0 };
	//transform_.rotate_ = { 0, 0, 0 };
	//transform_.scale_ = { 50, 50, 1 };  // ‰æ‘œƒTƒCƒY

	transform_.position_ = { 10,-500,0 };
	transform_.scale_ = { 0.2,0.2,1 };
	Image::SetTransform(hPic_, transform_);
	Image::Draw(hPic_);
}

void ResultHeader::Release()
{
}
