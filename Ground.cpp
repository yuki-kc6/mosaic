#include "Ground.h"
#include "Engine/Model.h"
#include "Engine/Direct3D.h"
#include "RenderTexture.h"
//コンストラクタ
Ground::Ground(GameObject* parent)
    :PaintObject(parent, "Ground")
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
    SetBrushSize(0.005f);
	transform_.position_.y = -0.8f;
	transform_.scale_ = { 10.0f,10.0f,10.0f };
}

//更新
void Ground::Update()
{
}

//描画
void Ground::Draw()
{
    Model::SetTransform(hModel_, transform_,this->GetMosaicRT()->GetShaderResourceView());
    Model::Draw(hModel_);

}



//開放
void Ground::Release()
{
}