 #include "TitleHeader.h"
#include "Engine/Image.h"
#include "Engine/Camera.h"
#include "Engine/Direct3D.h"

#include <sstream>
#include <Windows.h>

namespace
{
	constexpr XMFLOAT3 TITLE_POSITION = { 3.0f, 500.0f, 0.0f };
	constexpr XMFLOAT3 TITLE_SCALE = { 1.0f, 1.0f, 1.0f };

	constexpr XMFLOAT3 PUSH_START_POSITION = { 0.0f, -600.0f, 0.0f };
	constexpr XMFLOAT3 PUSH_START_SCALE = { 0.2f, 0.2f, 1.0f };

	constexpr XMFLOAT3 END_BUTTON_POSITION = { 1500.0f, -850.0f, 0.0f };
	constexpr XMFLOAT3 END_BUTTON_SCALE = { 0.2f, 0.2f, 0.0f };
	constexpr float END_BUTTON_IMAGE_WIDTH = 1024;
	constexpr float END_BUTTON_IMAGE_HEIGHT = 512;
}

TitleHeader::TitleHeader(GameObject* parent)
	:GameObject(parent,"TitleHeader"), hTitle_(-1), hPic_(-1),hEndButton_(-1)
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
	hEndButton_ = Image::Load("EndButton.png");
	assert(hEndButton_ >= 0);
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

	//終了ボタンの表示
	Transform button;
	button.position_ = END_BUTTON_POSITION;
	button.scale_ = END_BUTTON_SCALE;
	button.Calclation();
	Image::SetTransform(hEndButton_, button);
	Image::Draw(hEndButton_);
	
	
}

void TitleHeader::Release()
{
}

void TitleHeader::ButtonClick(XMFLOAT2 mousePos)
{
	//終了ボタンをクリックしたときの当たり判定

	//ボタンのポジション
	XMFLOAT3 pos = END_BUTTON_POSITION;

	//横と縦を計算
	float width = END_BUTTON_IMAGE_WIDTH * END_BUTTON_SCALE.x;
	float height = END_BUTTON_IMAGE_HEIGHT * END_BUTTON_SCALE.y;

	float left = pos.x - width * 0.5f;
	float right = pos.x + width * 0.5f;

	float top = pos.y + height * 0.5f;
	float bottom = pos.y - height * 0.5f;

	//マウス座標をもらってくる
	POINT mouse;
	mouse.x = mousePos.x;
	mouse.y = mousePos.y;

	//二倍で	Transformと合わせる
	float mx = mousePos.x *2;
	float my = mousePos.y * 2;


	//判定で終了
	if (mx >= left &&
		mx <= right &&
		my <= top &&
		my >= bottom)
	{
		PostQuitMessage(0);
	}
}
