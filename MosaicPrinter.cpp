#include <d3dcompiler.h>
#include "MosaicPrinter.h"
#include "Engine/Global.h"

namespace MosaicPrinter
{
	//元のRenderTargetの情報を入れておくための変数
	ID3D11RenderTargetView* originRTV = nullptr;
	ID3D11DepthStencilView* originDSV = nullptr;
	D3D11_VIEWPORT          originVP;
	ID3D11BlendState*		pBlendStateMax = nullptr;

	ID3D11InputLayout* pVertexLayout=nullptr;
	ID3D11VertexShader* pVertexShader=nullptr;
	ID3D11PixelShader* pPixelShader=nullptr;
	ID3D11RasterizerState* pRasterizerState=nullptr;

	ID3D11BlendState* pBlendState[Direct3D::BLEND_MAX];

	void MosaicPrinter::Initialize()
	{
		// Initialize内でのブレンドステート作成例
		D3D11_BLEND_DESC blendDesc = {};
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE; // 足し算（あるいは D3D11_BLEND_OP_MAX を使う）
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		Direct3D::pDevice_->CreateBlendState(&blendDesc, &pBlendStateMax);

		InitShader();
	}

	void MosaicPrinter::BeginPaint(RenderTexture* target)
	{
		//現在の設定の保存
		UINT numViewports = 1;
		Direct3D::pContext_->OMGetRenderTargets(1, &originRTV, &originDSV);
		Direct3D::pContext_->RSGetViewports(&numViewports,&originVP);

		ID3D11RenderTargetView* currentRTV = nullptr;
		Direct3D::pContext_->OMGetRenderTargets(1, &currentRTV, nullptr); // 今のRTVを取得
		Direct3D::pContext_->OMSetRenderTargets(1, &currentRTV, nullptr); // DSVなしで再設定

		if (currentRTV) currentRTV->Release(); // 取得した分をリリース

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
		if (originDSV) { originDSV->Release(); originDSV = nullptr; }


		float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
		Direct3D::pContext_->OMSetBlendState(pBlendState[Direct3D::BLEND_DEFAULT], blendFactor, 0xffffffff);

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
		vp.Width = w;
		vp.Height = h;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;

		Direct3D::pContext_->RSSetViewports(1, &vp);

		ShaderSet();

		//Direct3D::pContext_->Draw(2, 0);
	}


	void ShaderSet()
	{
		auto context = Direct3D::pContext_;

		// シェーダー切り替え
		context->VSSetShader(pVertexShader, nullptr, 0);
		context->PSSetShader(pPixelShader, nullptr, 0);

		// レイアウトは不要 (SV_VertexIDを使うため)
		context->IASetInputLayout(nullptr);

		// ブレンドステート適用 (Max合成)
		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		context->OMSetBlendState(pBlendStateMax, blendFactor, 0xffffffff);

		// ラスタライザ (カリングなし推奨)
		context->RSSetState(pRasterizerState);

		// トポロジー (TriangleStrip)
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	}

	void InitShader()
	{
		Direct3D::SetShader(Direct3D::SHADER_MASK);
	}

}