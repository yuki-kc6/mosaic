#include "ResultScene.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "ResultHeader.h"

//コンストラクタ
ResultScene::ResultScene(GameObject* parent)
	: GameObject(parent, "ResultScene")
{

}

//初期化
void ResultScene::Initialize()
{
	Instantiate<ResultHeader>(this);
}

//更新
void ResultScene::Update()
{
	if (Input::IsKeyDown(DIK_SPACE))
	{
		SceneManager* sm = (SceneManager*)FindObject("SceneManager");
		sm->ChangeScene(SCENE_ID_TITLE);
	}
}

//描画
void ResultScene::Draw()
{

}

//開放
void ResultScene::Release()
{

}
