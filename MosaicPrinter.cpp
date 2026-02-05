#include <d3dcompiler.h>
#include "MosaicPrinter.h"
#include "Engine/Global.h"

namespace MosaicPrinter
{
	//元のRenderTargetの情報を入れておくための変数
	ID3D11RenderTargetView* originRTV = nullptr;
	ID3D11DepthStencilView* originDSV = nullptr;
	D3D11_VIEWPORT          originVP;





	void MosaicPrinter::Initialize()
	{
	}

	void MosaicPrinter::BeginPaint(RenderTexture* target)
	{
		//現在の設定の保存
		UINT numViewports = 1;
		Direct3D::pContext_->OMGetRenderTargets(1, &originRTV, &originDSV);
		Direct3D::pContext_->RSGetViewports(&numViewports,&originVP);


		//RenderTargetの変更
		target->SetRenderTarget(Direct3D::pContext_);

		//Zバッファへの書き込みOFF
		Direct3D::SetDepthBafferWriteEnable(false);


	}

	void MosaicPrinter::EndPaint()
	{
		Direct3D::pContext_->OMSetRenderTargets(1, &originRTV, originDSV);
		Direct3D::pContext_->RSSetViewports(1, &originVP);


		if (originRTV) { originRTV->Release(); originRTV= nullptr; }
		if (originDSV) { originRTV->Release(); originDSV = nullptr; }

		Direct3D::SetDepthBafferWriteEnable(true);
	}

	void MosaicPrinter::Release()
	{
	}

	void MosaicPrinter::Paint(RenderTexture* targetRT, XMFLOAT2 hitUV)
	{
		int w  =targetRT->GetTextureWidth();
		int h = targetRT->GetTextureHeight();

		float px = hitUV.x * w;
		float py = hitUV.y * h;

		float brushSize = 64.0f;

		D3D11_VIEWPORT vp{};
		vp.TopLeftX = px - brushSize * 0.5f;
		vp.TopLeftY = py - brushSize * 0.5f;
		vp.Width = brushSize;
		vp.Height = brushSize;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;

		Direct3D::pContext_->RSSetViewports(1, &vp);

		ShaderSet();


	}


	void ShaderSet()
	{


	}
}