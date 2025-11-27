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
   
    move_ = XMVectorSet(0, 100.0f, 0, 0);


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
        Model::RayCast(tModel, &data); //レイを発射

    
        //レイが当たったら
        if (data.hit)
        {
            XMVECTOR s = XMLoadFloat3(&data.start);
            XMVECTOR d = XMLoadFloat3(&data.dir);

            XMVECTOR hitPos = XMVectorAdd(s, XMVectorScale(d, data.dist));

            XMFLOAT3 hit;
            XMStoreFloat3(&hit, hitPos);

            XMVECTOR p1 = XMLoadFloat3(&data.ver[0]);
            XMVECTOR p2 = XMLoadFloat3(&data.ver[1]);
            XMVECTOR p3 = XMLoadFloat3(&data.ver[2]);

            XMVECTOR v1 = p2 - p1;
            XMVECTOR v2 = p3 - p1;
            XMVECTOR vp = hitPos - p1;

            XMVECTOR vn = XMVector3Cross(v1, v2);
            XMVECTOR dot = XMVector3Dot(vp, vn);
            float suc = XMVectorGetX(dot);

            if (!suc)
            {
                XMVECTOR a = XMVector3Normalize(XMVector3Cross(p1 - p3, hitPos - p1));
                XMVECTOR b = XMVector3Normalize(XMVector3Cross(p2 - p1, hitPos - p2));
                XMVECTOR c = XMVector3Normalize(XMVector3Cross(p3 - p2, hitPos - p3));

                XMVECTOR d_ab = XMVector3Dot(a, b);
                XMVECTOR d_bc = XMVector3Dot(b, c);

                float suc1 = XMVectorGetX(d_ab);
                float suc2 = XMVectorGetX(d_bc);

             
                if (suc1=suc2)
                {
                    XMVECTOR uv1 = XMLoadFloat3(&data.ver[0]);
                    XMVECTOR uv2 = XMLoadFloat3(&data.ver[1]);
                    XMVECTOR uv3 = XMLoadFloat3(&data.ver[2]);

                    XMMATRIX mvp = Camera::GetProjectionMatrix() * Camera::GetViewMatrix() * GetWorldMatrix();

                    XMVECTOR p1_p = XMVector3TransformCoord(p1, mvp);
                    XMVECTOR p2_p = XMVector3TransformCoord(p2, mvp);
                    XMVECTOR p3_p = XMVector3TransformCoord(p3, mvp);
                    XMVECTOR p_p = XMVector3TransformCoord(hitPos, mvp);

                    XMFLOAT2 p1_n=

                }
                
                
            }


            
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

void Bullet::SetStart(XMFLOAT3 start)
{
    data.start = start;
}



