#include "Building.h"

#include "Engine/BoxCollider.h"
#include "Engine/Model.h"
#include "Engine/Direct3D.h"
#include "RenderTexture.h"
//コンストラクタ
Building::Building(GameObject* parent)
    :PaintObject(parent, "Building")
{
}

//デストラクタ
Building::~Building()
{
}

//初期化
void Building::Initialize()
{
    hModel_ = Model::Load("Models/building1.fbx");
    assert(hModel_ >= 0);
    //transform_.position_.y = ;
    //this->SetSensitive(true);
    BoxCollider* collision = new BoxCollider(transform_.position_, XMFLOAT3(30, 30, 30));
    AddCollider(collision);
    SetBrushSize(0.03f);
	transform_.scale_ = { 5.0f,5.0f,5.0f };
}

//更新
void Building::Update()
{
}

//描画
void Building::Draw()
{
    Model::SetTransform(hModel_, transform_, this->GetMosaicRT()->GetShaderResourceView());
    Model::Draw(hModel_);
}



//開放
void Building::Release()
{
}