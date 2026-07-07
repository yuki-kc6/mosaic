#include "TitleHeader.h"
#include "Engine/Image.h"

namespace
{
	constexpr XMFLOAT3 TITLE_POSITION = { 3.0f, 500.0f, 0.0f };
	constexpr XMFLOAT3 TITLE_SCALE = { 1.0f, 1.0f, 1.0f };

	constexpr XMFLOAT3 PUSH_START_POSITION = { 0.0f, -600.0f, 0.0f };
	constexpr XMFLOAT3 PUSH_START_SCALE = { 0.2f, 0.2f, 1.0f };
}

TitleHeader::TitleHeader(GameObject* parent)
	:GameObject(parent,"TitleHeader"), hTitle_(-1), hPic_(-1)
{
}

TitleHeader::~TitleHeader()
{
}

void TitleHeader::Initialize()
{
	//それぞれ画像をロード
	hPic_ = Image::Load("title.png");
	assert(hPic_ >= 0);
	hTitle_ = Image::Load("titleName.png");
	assert(hTitle_ >= 0);
}

void TitleHeader::Update()
{
}

void TitleHeader::Draw()
{
	//タイトル画像の表示
	Transform title;
	title.position_ = TITLE_POSITION;
	title.scale_ = TITLE_SCALE;
	Image::SetTransform(hTitle_, title);
	Image::Draw(hTitle_);

	//PushtoStartの表示
	Transform push;
	push.position_ = PUSH_START_POSITION;
	push.scale_ = PUSH_START_SCALE;
	Image::SetTransform(hPic_, push);
	Image::Draw(hPic_);

	
}

void TitleHeader::Release()
{
}
