#include "MosaicPrinter.h"
#include "Engine/Direct3D.h"
#include "Engine/Global.h"

void MosaicPrinter::Initialize()
{
}

void MosaicPrinter::BeginPaint(RenderTexture* target)
{
	target->SetRenderTarget(Direct3D::pContext_);

	Direct3D::SetDepthBafferWriteEnable(false);


}

void MosaicPrinter::EndPaint()
{
}

void MosaicPrinter::Release()
{
}

void MosaicPrinter::Paint(ID3D11DeviceContext* context, RenderTexture* targetRT, XMFLOAT2 hitUV)
{

}
