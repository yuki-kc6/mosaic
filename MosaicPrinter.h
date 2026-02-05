#pragma once
#include "RenderTexture.h"

//ƒŠƒ“ƒJ
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")


namespace MosaicPrinter
{
	void Initialize();

	void BeginPaint(RenderTexture* target);

	void EndPaint();

	void Release();

	void Paint(RenderTexture* targetRT, XMFLOAT2 hitUV);

	void ShaderSet();

}
