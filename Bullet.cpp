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

            XMVECTOR p1 = XMLoadFloat3(&data.ver[0]);
            XMVECTOR p2 = XMLoadFloat3(&data.ver[1]);
            XMVECTOR p3 = XMLoadFloat3(&data.ver[2]);
        
           
            
            XMFLOAT3 uv1 = data.uv[0];
            XMFLOAT3 uv2 = data.uv[1];
            XMFLOAT3 uv3 = data.uv[2];
            //透視投影を考慮したUV補間
            XMMATRIX mvp = Camera::GetProjectionMatrix() * Camera::GetViewMatrix() * pTarget_->GetWorldMatrix();

            //ワールド座標への変換
            XMVECTOR p1_p = XMVector4Transform(p1, mvp);
            XMVECTOR p2_p = XMVector4Transform(p2, mvp);
            XMVECTOR p3_p = XMVector4Transform(p3, mvp);
            XMVECTOR p_p = XMVector4Transform(vHitPos, mvp);

            float w1 = XMVectorGetW(p1_p);
            float w2 = XMVectorGetW(p2_p);
            float w3 = XMVectorGetW(p3_p);
            float wp = XMVectorGetW(p_p);


            Vector2 p1_n{(XMVectorGetX(p1_p) / w1),(XMVectorGetY(p1_p) / w1)};
            Vector2 p2_n{(XMVectorGetX(p2_p) / w2),(XMVectorGetY(p2_p) / w2)};
            Vector2 p3_n{(XMVectorGetX(p3_p) / w3),(XMVectorGetY(p3_p) / w3)};
            Vector2 p_n{(XMVectorGetX(p_p) / wp),(XMVectorGetY(p_p) / wp)};

            float  s = 0.5f * ((p2_n.x - p1_n.x) * (p3_n.y - p1_n.y) - (p2_n.y - p1_n.y) * (p3_n.x - p1_n.x));
            float s1 = 0.5f * ((p3_n.x - p_n.x) * (p1_n.y - p_n.y) - (p3_n.y - p_n.y) * (p1_n.x - p_n.x));
            float s2 = 0.5f * ((p1_n.x - p_n.x) * (p2_n.y - p_n.y) - (p1_n.y - p_n.y) * (p2_n.x - p_n.x));
            
            float b0 = 1.0f - u - v;
            float u = s1 / s;
            float v= s2 / s;
            float w = 1 / ((1 - u - v) * 1 / w1 + u * 1 / w2 + v * 1 / w3);

            XMVECTOR vUv1 = XMLoadFloat3(&uv1);
            XMVECTOR vUv2 = XMLoadFloat3(&uv2);
            XMVECTOR vUv3 = XMLoadFloat3(&uv3);

            XMVECTOR term1 = XMVectorScale(vUv1, b0 / w1);
            XMVECTOR term2 = XMVectorScale(vUv2, u / w2);
            XMVECTOR term3 = XMVectorScale(vUv3, v / w3);
            
            XMVECTOR sum = XMVectorAdd(XMVectorAdd(term1, term2), term3);

            XMVECTOR vUv = XMVectorScale(sum, w);
            
            XMFLOAT3 uv_result;
            XMStoreFloat3(&uv_result, vUv);



            pTarget_->PaintMosaic(uv_result);





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





