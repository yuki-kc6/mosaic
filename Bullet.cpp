#include "Bullet.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include "Engine/Camera.h"

Bullet::Bullet(GameObject* parent)
	:GameObject(parent, "Bullet"), hModel_(-1),life_(100),tModel(-1)
{

}

Bullet::~Bullet()
{

}

void Bullet::Initialize()
{
    hModel_ = Model::Load("Models/BulletKari.fbx");
    assert(hModel_ >= 0);

    SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), 1.2f);
    AddCollider(collision);
   
    XMFLOAT3 cameraPos = Camera::GetPosition();
    XMFLOAT3 targetPos = Camera::GetTarget();

    XMVECTOR vCameraTarget = XMLoadFloat3(&targetPos);
    XMVECTOR vCameraPos = XMLoadFloat3(&cameraPos);
    
    vCameraTarget = vCameraTarget - vCameraPos;
    
    move_ = XMVector3Normalize(vCameraTarget);
}

void Bullet::Update()
{
    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);

    vPos += move_;
    XMStoreFloat3(&transform_.position_, vPos);


    life_--;//毎フレーム1ずつ減る
    if (life_ <= 0)
    {
        KillMe();//死
    }


    if (tModel != -1)
    {
        XMFLOAT3 dir;
        XMStoreFloat3(&dir, move_);
        data.dir = dir;//レイの方向
        data.start = Camera::GetPosition();
        Model::RayCast(tModel, &data); //レイを発射

        
        //レイが当たったら
        if (data.hit)
        {
            pTarget_->KillMe();
            XMVECTOR vStart = XMLoadFloat3(&data.start);
            XMVECTOR vDir = XMLoadFloat3(&data.dir);

            XMVECTOR vHitPos = XMVectorAdd(vStart, XMVectorScale(vDir, data.dist));

            XMFLOAT3 hitPos;
            XMStoreFloat3(&hitPos, vHitPos);

            XMMATRIX decalWorld



        }
        else
        {
           // KillMe();
        }
    }
}
void Bullet::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);

}

void Bullet::Release()
{
}

void Bullet::OnCollision(GameObject* pTarget)
{
    pTarget_ = pTarget;
   tModel= pTarget->GetModelHandle();

}





