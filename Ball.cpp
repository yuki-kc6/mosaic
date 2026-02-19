#include "Ball.h"
#include "Engine/Model.h"
#include "Engine/Direct3D.h"
Ball::Ball(GameObject* parent)
	:PaintObject(parent, "Ball"), hModel_(-1)
{
}

Ball::~Ball()
{
}

void Ball::Initialize()
{
	hModel_ = Model::Load("Models/ball.fbx");
	assert(hModel_ >= 0);
}

void Ball::Update()
{
}

void Ball::Draw()
{
	Model::SetTransform(hModel_, transform_, this->GetMosaicRT()->GetShaderResourceView());
	Model::Draw(hModel_);
}

void Ball::Release()
{

}
