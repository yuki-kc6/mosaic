#include "PaintObject.h"
#include "Engine/Direct3D.h"
#include "Engine/FbxParts.h"


PaintObject::PaintObject()
	:GameObject(),mosaicRT(nullptr)
{
	mosaicRT = new RenderTexture();
	mosaicRT->Initialize(Direct3D::pDevice_, 512, 512, 0.1f, 1000.0f, 1);
	
}

PaintObject::PaintObject(GameObject* parent)
	:GameObject(parent), mosaicRT(nullptr)
{
	mosaicRT = new RenderTexture();
	mosaicRT->Initialize(Direct3D::pDevice_, 512, 512, 0.1f, 1000.0f, 1);
}

PaintObject::PaintObject(GameObject* parent, const std::string& name)
	:GameObject(parent,name), mosaicRT(nullptr)
{
	mosaicRT = new RenderTexture();
	mosaicRT->Initialize(Direct3D::pDevice_, 512, 512, 0.1f, 1000.0f, 1);
	mosaicRT->ClearRenderTarget(Direct3D::pContext_, 0.0, 0.0, 0.0, 1);
}



PaintObject::~PaintObject()
{
}



void PaintObject::PaintMosaic(XMFLOAT2 uv)
{
	MosaicPrinter::BeginPaint(mosaicRT);
	MosaicPrinter::Paint(mosaicRT, uv);
	MosaicPrinter::EndPaint();
}
