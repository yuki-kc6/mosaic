#include "Building.h"

#include "Engine/BoxCollider.h"
#include "Engine/Model.h"
#include "Engine/Direct3D.h"
#include "RenderTexture.h"

namespace
{
    constexpr float BUILDING_SCALE = 5.0f;
    constexpr float BUILDING_Y = -1.0f;
    constexpr float BRUSH_SIZE = 0.003f;
    constexpr float COLLIDER_SIZE = 25.0f;
}


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

    hModel_ = Model::Load("Models/building2.fbx");
    assert(hModel_ >= 0);
    
    BoxCollider* collision = new BoxCollider(transform_.position_, XMFLOAT3(COLLIDER_SIZE, COLLIDER_SIZE, COLLIDER_SIZE));
    AddCollider(collision);
    SetBrushSize(BRUSH_SIZE);
	transform_.scale_ = { BUILDING_SCALE, BUILDING_SCALE, BUILDING_SCALE };
    transform_.position_.y = BUILDING_Y;
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