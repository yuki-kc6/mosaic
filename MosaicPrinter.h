#pragma once
#include "RenderTexture.h"

//リンカ
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

struct MOSAIC_CONSTANT_BUFFER
{
	XMFLOAT2 center;  // 中心座標
	float radius;     // 塗りの半径
	float isCompleted;   // 全部塗る際に仕様

};

namespace MosaicPrinter
{
	void Initialize();

	void BeginPaint(RenderTexture* target);

	void EndPaint();

	void Release();

	void Paint(RenderTexture* targetRT, XMFLOAT2 hitUV, float brushSize,float paintAll);

	void ShaderSet();

	void InitShader();
}
