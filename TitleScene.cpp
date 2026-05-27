#include "TitleScene.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "TitleHeader.h"

//コンストラクタ
TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene")
{

}

//初期化
void TitleScene::Initialize()
{
	Instantiate<TitleHeader>(this);

	SceneManager* sm = (SceneManager*)FindObject("SceneManager");
	sm->ChangeScene(SCENE_ID_PLAY);


}

//更新
void TitleScene::Update()
{
	if (Input::IsKeyDown(DIK_SPACE))
	{
		SceneManager* sm = (SceneManager*)FindObject("SceneManager");
		sm->ChangeScene(SCENE_ID_PLAY);
	}
}

//描画
void TitleScene::Draw()
{

}

//開放
void TitleScene::Release()
{

}
