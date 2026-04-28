#include "Controller.h"
#include "Engine/Input.h"

Controller::Controller(GameObject* parent)
{
}

Controller::~Controller()
{
}

void Controller::Initialize()
{
}

void Controller::Update()
{
        XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
        XMMATRIX mRotate = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
        XMVECTOR vMoveForward = { 0,0,1,0 };
        XMVECTOR vMoveRight = { 1,0,0,0 };

        vMoveForward = XMVector3TransformNormal(vMoveForward, mRotate);
        vMoveRight = XMVector3TransformNormal(vMoveRight, mRotate);

        //移動
        if (Input::IsKey(DIK_W))
        {
            vPos += vMoveForward * moveSpeed_;
        }
        if (Input::IsKey(DIK_S))
        {
            vPos -= vMoveForward * moveSpeed_;
        }
        if (Input::IsKey(DIK_D))
        {
            vPos += vMoveRight * moveSpeed_;
        }
        if (Input::IsKey(DIK_A))
        {
            vPos -= vMoveRight * moveSpeed_;
        }
        XMStoreFloat3(&transform_.position_, vPos);


    if (Input::IsKey(DIK_SPACE))
    {
        transform_.position_.y += 0.3;
    }
    if (Input::IsKey(DIK_B))
    {
        transform_.position_.y -= 0.3;
    }
}

void Controller::Draw()
{
}

void Controller::Release()
{
}
