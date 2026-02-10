#include "PlayScene.h"
#include "Ground.h"
#include "Player.h"
#include "Enemy.h"
#include "MosaicPrinter.h"


//コンストラクタ
PlayScene::PlayScene(GameObject * parent)
	: GameObject(parent, "PlayScene")
{
}

//初期化
void PlayScene::Initialize()
{
	MosaicPrinter::Initialize();
	Instantiate<Ground>(this);
	Instantiate<Player>(this);
	Instantiate<Enemy>(this);
	
}

//更新
void PlayScene::Update()
{
}

//描画
void PlayScene::Draw()
{
}

//開放
void PlayScene::Release()
{
	MosaicPrinter::Release();
}
