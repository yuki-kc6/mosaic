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
	//初期化
	void Initialize();

	//ペイント開始
	void BeginPaint(RenderTexture* target);

	//ペイント終了
	void EndPaint();

	//解放
	void Release();

	//ペイントする
	//引数：
	// targetRT: 描画対象のRenderTexture
	// hitUV: 塗る位置のUV座標
	// brushSize: ブラシの大きさ
	// paintAll: 全部塗るかどうか
	void Paint(RenderTexture* targetRT, XMFLOAT2 hitUV, float brushSize,float paintAll);
}
