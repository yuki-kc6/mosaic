#include "Wall.h"
#include "Engine/Model.h"
#include "Engine/Direct3D.h"
#include "RenderTexture.h"
//コンストラクタ
Wall::Wall(GameObject* parent)
    :PaintObject(parent, "Wall")
{
}

//デストラクタ
Wall::~Wall()
{
}

//初期化
void Wall::Initialize()
{
    hModel_ = Model::Load("Models/building.fbx");
    assert(hModel_ >= 0);
    transform_.position_.y = 10.;
}

//更新
void Wall::Update()
{
}

//描画
void Wall::Draw()
{
    Model::SetTransform(hModel_, transform_, this->GetMosaicRT()->GetShaderResourceView());
    Model::Draw(hModel_);
}



//開放
void Wall::Release()
{
}