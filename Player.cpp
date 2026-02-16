#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Ground.h"
#include "Engine/Camera.h"
#include "Bullet.h"
#include "RenderTexture.h"


namespace
{

}


//コンストラクタ
Player::Player(GameObject* parent)
    :GameObject(parent, "Player"), hModel_(-1)
{

}

//デストラクタ
Player::~Player()
{
}

//初期化
void Player::Initialize()
{
    hModel_ = Model::Load("Models/PlayerKari.fbx");
    assert(hModel_ >= 0);

    transform_.position_.y = 1;
    Input::SetMousePosition(50, 50);
    camTarY = 3.0;
    gravity = 3.0;


}

//更新
void Player::Update()
{

    mousePos = Input::GetMousePosition();//現在のマウスの座標

    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
    XMVECTOR vMove1 = { 0,0,0.1f,0 };
    XMVECTOR vMove2 = { 0.1f,0,0,0 };
    XMVECTOR vMove3 = { 0,1.0f,0,0 };


    XMMATRIX mRotate = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));


    vMove1 = XMVector3TransformCoord(vMove1, mRotate);
    vMove2 = XMVector3TransformCoord(vMove2, mRotate);
    vMove3 = XMVector3TransformCoord(vMove3, mRotate);

    

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


    //横軸の視点移動
    if (Input::GetMouseMove().x)
    {
        transform_.rotate_.y += (mousePos.x - PrevMousePos.x) * 0.5;
    }

    //縦軸の視点移動
    if (Input::GetMouseMove().y&&camTarY<100&&camTarY>0)
    {
        camTarY -= (mousePos.y - PrevMousePos.y) * 0.01;
        transform_.rotate_.x = (mousePos.y- PrevMousePos.y) * 0.01;
    }

    PrevMousePos = mousePos;//PrevMousePosの更新

    XMStoreFloat3(&transform_.position_, vPos);

    

    XMVECTOR vCam = { 0, 3.0f, 5.0f, 0 };//カメラ位置ベクトル
    XMVECTOR vCamT = { 0, camTarY,6.0f, 0 };//カメラターゲットのベクトル


    //カメラ位置をセット
    vCam = XMVector3TransformCoord(vCam, mRotate);
    XMFLOAT3 camPos;
    XMStoreFloat3(&camPos, vPos + vCam);
    Camera::SetPosition(camPos);


    //カメラターゲットのセット
    vCamT = XMVector3TransformCoord(vCamT, mRotate);
    XMFLOAT3 camTarget;
    XMStoreFloat3(&camTarget, vPos + vCamT);
    Camera::SetTarget(camTarget);

    XMVECTOR vBullet = vCamT - vCam;


    Ground* pGround = (Ground*)FindObject("Ground");    //ステージオブジェクトを探す
    int hGroundModel = pGround->GetModelHandle();    //モデル番号を取得

    XMFLOAT2 testUV = { 50.0f, 50.0f };

    if (Input::IsMouseButtonDown(0))
    {
      // Bullet* pBullet = Instantiate<Bullet>(GetParent());
      // pBullet->SetPosition(Camera::GetPosition());
      //// pBullet->SetStart(transform_.position_);
      // //pBullet->SetMove(vBullet);
		XMFLOAT2 hitUV;
        hitUV.x = transform_.position_.x;
        hitUV.y= transform_.position_.z;
		pGround->PaintMosaic(testUV);//タイルを塗る
        
    }

 
   

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