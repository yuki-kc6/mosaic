#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <wincodec.h>
#include "string"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "LibFbxSDK-MT.lib")
#pragma comment(lib, "LibXml2-MT.lib")
#pragma comment(lib, "zlib-MT.lib")
#pragma comment( lib, "WindowsCodecs.lib" )

using namespace DirectX;


class RenderTexture
{
public:
	RenderTexture();
	RenderTexture(const RenderTexture&);
	~RenderTexture();

	bool Initialize(ID3D11Device* texture, int texWidth, int texHeight, float screenDepth, float screenNear, int format);
	void Shutdown();


	void SetRenderTarget(ID3D11DeviceContext* deviceContext);
	void ClearRenderTarget(ID3D11DeviceContext* deviceContext, float red, float green, float blue, float alpha);
	
	ID3D11ShaderResourceView* GetShaderResourceView() { return pShaderResourceView_; }	//シェーダーリソースビューの取得
	XMMATRIX GetOrthoMatrix() { return orthoMatrix; } // ペイント描画時に使います

	void GetProjectionMatrix(XMMATRIX);
	void GetOrthoMatrix(XMMATRIX);

	int GetTextureWidth();
	int GetTextureHeight();

	

private:
	int textureWidth;
	int textureHeight;


	ID3D11Texture2D* pRenderTargetTexture_;
	ID3D11RenderTargetView* pRenderTargetView_;
	ID3D11ShaderResourceView* pShaderResourceView_;
	ID3D11Texture2D* pDepthStencilBuffer_;
	ID3D11DepthStencilView* pDepthStencilView_;
	D3D11_VIEWPORT viewPort_;
	XMMATRIX projectionMatrix;
	XMMATRIX orthoMatrix;


	ID3D11RenderTargetView* pOldRTV_ = nullptr;
	ID3D11DepthStencilView* pOldDSV_ = nullptr;



};

