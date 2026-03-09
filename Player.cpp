#include "Player.h"
#include "Engine/Input.h"
#include "Ground.h"
#include "Engine/Camera.h"
#include "Bullet.h"
#include "RenderTexture.h"

//コンストラクタ
Player::Player(GameObject* parent)
    :GameObject(parent, "Player"), hModel_(-1)
{

}

//デストラクタ
Player::~Player()
{
    ShowCursor(TRUE);
}

//初期化
void Player::Initialize()
{
    SetCursorPos(50, 50);
    hModel_ = Model::Load("Models/PlayerKari.fbx");
    assert(hModel_ >= 0);

    transform_.position_.y = 1;
    Input::SetMousePosition(50, 30);
    mousePos = Input::GetMousePosition();
    camTarY = 3.0;
    gravity = 3.0;
    paintObj = nullptr;
    centerX = GetSystemMetrics(SM_CXSCREEN) / 2;
    centerY = GetSystemMetrics(SM_CYSCREEN) / 2;

    //ShowCursor(FALSE);
}

//更新
void Player::Update()
{
    mousePos = Input::GetMousePosition();
   
    this->FPSCamera();
    this->PlayerMove();

    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);

    RayCastData gan;
	gan.start=Camera::GetPosition();
	gan.dir=Camera::GetTarget();
	
    if (Input::IsMouseButton(0))
    {
        this->RayCastToPaintObjects(gan);
    }

    this->OnGround();
}

//描画
void Player::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
    
}

//開放
void Player::Release()
{
}

void Player::FPSCamera()
{
    SetCursorPos(centerX, centerY);

    if (Input::GetMouseMove)
    {
        //横軸の視点移動
        if (Input::GetMouseMove().x)
        {
            transform_.rotate_.y += (baseMousePos.x - currentMousePos.x) * 0.5;
        }

        //縦軸の視点移動
        if (Input::GetMouseMove().y && camTarY < 100 && camTarY>0)
        {
            camTarY -= (baseMousePos.y - currentMousePos.y) * 0.01;
            transform_.rotate_.x = (baseMousePos.y - currentMousePos.y) * 0.01;
        }
    }
}

void Player::OnGround()
{
    Ground* pGround = (Ground*)FindObject("Ground");    //ステージオブジェクトを探す
    int hGroundModel = pGround->GetModelHandle();    //モデル番号を取得

    RayCastData data;
    data.start = transform_.position_;   //レイの発射位置
    data.start.y = 0;

    data.dir = XMFLOAT3(0, -1, 0);       //レイの方向
    Model::RayCast(hGroundModel, &data); //レイを発射

    //レイが当たったら
    if (data.hit)
    {
        //その分位置を下げる
        transform_.position_.y = -data.dist;//当たった距離のマイナス、ステージの最高点が0より低い
    }
}

void Player::RayCastToPaintObjects(RayCastData& data)
{
    PaintObject* closestObj = nullptr;
    float dist = FLT_MAX;
    XMFLOAT2 UV = { 0,0 };
    // PaintObject 継承クラスだけに絞ってループ
    for (PaintObject* pObj : PaintObject::GetPaintObjectList()) {
        // 描画されていない、または死んでいるオブジェクトはスキップ
        RayCastData ray = data; // コピーして使用
        if (!pObj->IsVisibled() || pObj->IsDead()) continue;
        // 親クラス GameObject の hModel_ と WorldMatrix を使用して判定
        Model::RayCast(pObj->GetModelHandle(), &ray);

        if (ray.hit)
        {
            if (dist > ray.dist)
            {
                dist = ray.dist;
                closestObj = pObj;
                UV = ray.uv;
            }
        }
    }
    if (closestObj != nullptr)
    {
        closestObj->PaintMosaic(UV);
    }
}

void Player::PlayerMove()
{
    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
    XMVECTOR vMove1 = { 0,0,0.1f,0 };
    XMVECTOR vMove2 = { 0.1f,0,0,0 };
    XMVECTOR vMove3 = { 0,1.0f,0,0 };


    XMMATRIX mRotate = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));


    vMove1 = XMVector3TransformCoord(vMove1, mRotate);
    vMove2 = XMVector3TransformCoord(vMove2, mRotate);
    vMove3 = XMVector3TransformCoord(vMove3, mRotate);


    // this->FPSCamera();

     //移動
    {
        if (Input::IsKey(DIK_W))
        {
            vPos += vMove1;
            XMStoreFloat3(&transform_.position_, vPos);
        }
        if (Input::IsKey(DIK_S))
        {
            vPos -= vMove1;
            XMStoreFloat3(&transform_.position_, vPos);
        }
        if (Input::IsKey(DIK_D))
        {
            vPos += vMove2;
            XMStoreFloat3(&transform_.position_, vPos);
        }
        if (Input::IsKey(DIK_A))
        {
            vPos -= vMove2;
            XMStoreFloat3(&transform_.position_, vPos);
        }
    }

    XMStoreFloat3(&transform_.position_, vPos);
}
