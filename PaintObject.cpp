#include "PaintObject.h"
#include "Engine/Direct3D.h"

PaintObject::PaintObject()
	:GameObject(),mosaicRT(nullptr)
{
}

PaintObject::PaintObject(GameObject* parent)
	:GameObject(parent), mosaicRT(nullptr)
{
}

PaintObject::PaintObject(GameObject* parent, const std::string& name)
	:GameObject(parent,name), mosaicRT(nullptr)
{
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
