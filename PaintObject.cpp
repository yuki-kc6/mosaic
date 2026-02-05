#include "PaintObject.h"
#include "Engine/Direct3D.h"

PaintObject::PaintObject()
	:GameObject()
{
}

PaintObject::PaintObject(GameObject* parent)
	:GameObject(parent)
{
}

PaintObject::PaintObject(GameObject* parent, const std::string& name)
	:GameObject(parent,name)
{
}


void PaintObject::PaintMosaic(XMFLOAT2 uv)
{
	MosaicPrinter::BeginPaint();
	MosaicPrinter::Paint(Direct3D::pContext_, mosaicRT, uv);
	MosaicPrinter::EndPaint();
}
