#include "Ground.h"
#include "Engine/Model.h"
//コンストラクタ
Ground::Ground(GameObject* parent)
    :GameObject(parent, "Ground"),hModel_(-1)
{
}

//デストラクタ
Ground::~Ground()
{
}

//初期化
void Ground::Initialize()
{
    hModel_ = Model::Load("Models/map1.fbx");
    assert(hModel_ >= 0);

    paintMap = new RenderTexture();

    paintMap->Create(1024, 1024);

    // 初期状態は「未塗装」
    paintMap->Begin();
   

    paintMap->Clear(1, 0, 0, 1);   // 真っ赤

    paintMap->End();
}

//更新
void Ground::Update()
{
}

//描画
void Ground::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//開放
void Ground::Release()
{
}