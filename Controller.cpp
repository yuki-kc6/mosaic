#include "Controller.h"
#include "Engine/Input.h"

Controller::Controller(GameObject* parent)
    :GameObject(parent,"Controlle")
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
     
}

void Controller::Draw()
{
}

void Controller::Release()
{
}

XMFLOAT2 Controller::GetMoveInput()
{
    //今はまだキーボード入力を返す
    XMFLOAT2 move = {};

    if (Input::IsKey(DIK_W))
        move.y += 1;

    if (Input::IsKey(DIK_S))
        move.y -= 1;

    if (Input::IsKey(DIK_D))
        move.x += 1;

    if (Input::IsKey(DIK_A))
        move.x -= 1;

    return move;
}
