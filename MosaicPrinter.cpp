#include <d3dcompiler.h>
#include "MosaicPrinter.h"
#include "Engine/Global.h"

namespace MosaicPrinter
{
	//元のRenderTargetの情報を入れておくための変数
	ID3D11RenderTargetView* originRTV = nullptr;
	ID3D11DepthStencilView* originDSV = nullptr;
	D3D11_VIEWPORT          originVP;
	ID3D11BlendState* pBlendStateMax = nullptr;

	//シェーダーの設定
	ID3D11InputLayout* pVertexLayout = nullptr;
	ID3D11VertexShader* pVertexShader = nullptr;
	ID3D11PixelShader* pPixelShader = nullptr;
	ID3D11RasterizerState* pRasterizerState = nullptr;

	//ブレンドステートの設定
	ID3D11BlendState* pBlendState[Direct3D::BLEND_MAX];

	//定数バッファの設定
	ID3D11Buffer* pConstantBuffer_;


	void MosaicPrinter::Initialize()
	{
		//必要な設定項目
		D3D11_BUFFER_DESC cb;
		cb.ByteWidth = sizeof(MOSAIC_CONSTANT_BUFFER);
		cb.Usage = D3D11_USAGE_DYNAMIC;
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb.MiscFlags = 0;
		cb.StructureByteStride = 0;

		// 定数バッファの作成
		Direct3D::pDevice_->CreateBuffer(&cb, NULL, &pConstantBuffer_);
	}

	void MosaicPrinter::BeginPaint(RenderTexture* target)
	{
		//シェーダーをモザイク書き込み用に変更
		Direct3D::SetShader(Direct3D::SHADER_MASK);

		//現在の設定の保存
		UINT numViewports = 1;
		Direct3D::pContext_->OMGetRenderTargets(1, &originRTV, &originDSV);
		Direct3D::pContext_->RSGetViewports(&numViewports, &originVP);


		ID3D11ShaderResourceView* nullSRV[2] = { nullptr, nullptr };
		Direct3D::pContext_->PSSetShaderResources(1, 1, nullSRV);

		//RenderTargetの変更
		target->SetRenderTarget(Direct3D::pContext_);

		Direct3D::pContext_->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	}

	void MosaicPrinter::EndPaint()
	{
		//元のRenderTargetに戻す
		Direct3D::SetDepthBafferWriteEnable(true);
		Direct3D::pContext_->RSSetViewports(1, &originVP);
		Direct3D::pContext_->OMSetRenderTargets(1, &originRTV, originDSV);

		//元のRenderTargetの解放
		if (originRTV) { originRTV->Release(); originRTV = nullptr; }
		if (originDSV) { originDSV->Release(); originDSV = nullptr; }
		

		//ブレンドステートの設定
		float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
		Direct3D::pContext_->OMSetBlendState(pBlendState[Direct3D::BLEND_DEFAULT], blendFactor, 0xffffffff);

		//シェーダーを戻す
		Direct3D::SetShader(Direct3D::SHADER_3D);

	}

	void MosaicPrinter::Release()
	{
	}

	void MosaicPrinter::Paint(RenderTexture* targetRT, XMFLOAT2 hitUV,float brushSize,float paintAll)
	{
		
		Direct3D::pContext_->PSSetConstantBuffers(0, 1, &pConstantBuffer_);

		// 定数バッファの設定
		MOSAIC_CONSTANT_BUFFER cb;
		D3D11_MAPPED_SUBRESOURCE pdata;
		cb.center = hitUV;// 中心座標を設定
		cb.radius = brushSize;// 半径を設定
		cb.isCompleted = paintAll;// 全部塗るかどうかを設定


		Direct3D::pContext_->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);

		memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));		// リソースへ値を送る

		Direct3D::pContext_->Unmap(pConstantBuffer_, 0);

		// ビューポート
		D3D11_VIEWPORT vp{};
		vp.Width = (float)targetRT->GetTextureWidth();
		vp.Height = (float)targetRT->GetTextureHeight();
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		Direct3D::pContext_->RSSetViewports(1, &vp);

		int vertex = 4;
		Direct3D::pContext_->Draw(vertex, 0);



	}

}