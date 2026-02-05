#pragma once
#include "RenderTexture.h"



namespace MosaicPrinter
{
	void Initialize();

	void BeginPaint(RenderTexture* target);

	void EndPaint();

	void Release();

	void Paint(ID3D11DeviceContext* context, RenderTexture* targetRT, XMFLOAT2 hitUV);

}
