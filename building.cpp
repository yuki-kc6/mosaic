#include "Building.h"

#include "Engine/BoxCollider.h"
#include "Engine/Model.h"
#include "Engine/Direct3D.h"
#include "RenderTexture.h"

namespace
{
    constexpr float BUILDING_SCALE = 5.0f;//ビルの大きさ
    constexpr float BUILDING_Y = -3.0f;//ビルのY座標
    constexpr float BRUSH_SIZE = 0.003f;//ビルに塗るブラシの大きさ
	constexpr float COLLIDER_SIZE = 25.0f;//ビルの当たり判定の大きさ
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

    hModel_ = Model::Load("Models/building2.fbx");//モデル読み込み
    assert(hModel_ >= 0);
    
    //四角当たり判定の追加
    BoxCollider* collision = new BoxCollider(transform_.position_, XMFLOAT3(COLLIDER_SIZE, COLLIDER_SIZE, COLLIDER_SIZE));
    AddCollider(collision);

    SetBrushSize(BRUSH_SIZE);//ブラシサイズの設定
	transform_.scale_ = { BUILDING_SCALE, BUILDING_SCALE, BUILDING_SCALE };//ビルの大きさを設定
	transform_.position_.y = BUILDING_Y;//ビルのY座標を設定
}

//更新
void Building::Update()
{
	
}

//描画
void Building::Draw()
{
	Model::SetTransform(hModel_, transform_, this->GetMosaicRT()->GetShaderResourceView());//ペイントオブジェクト用の描画関数を呼ぶ
    Model::Draw(hModel_);
}



//開放
void Building::Release()
{
}