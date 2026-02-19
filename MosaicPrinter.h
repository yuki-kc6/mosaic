#pragma once
#include "RenderTexture.h"

//リンカ
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

struct CONSTANT_BUFFER
{
	XMFLOAT2 center;  // 8 bytes
	float radius;     // 4 bytes
	float padding;   // 4 bytes（16バイト揃え）
};




namespace MosaicPrinter
{
	
	void Initialize();

	void BeginPaint(RenderTexture* target);

	void EndPaint();

	void Release();

	void Paint(RenderTexture* targetRT, XMFLOAT2 hitUV);

	void ShaderSet();

	void InitShader();
}


void InitConstatnrBuffer();