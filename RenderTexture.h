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
	bool Create(int w, int h);
	void Begin();
	void End();

	//各アクセス関数
	ID3D11ShaderResourceView* GetSRV() { return pRenderTextureSRV_; }	//シェーダーリソースビューの取得

private:
	ID3D11Texture2D* pTexure_=nullptr;
	ID3D11RenderTargetView* pRenderTextureRTV_=nullptr;
	ID3D11ShaderResourceView* pRenderTextureSRV_ = nullptr;

	ID3D11RenderTargetView* pOldRTV_ = nullptr;
	ID3D11DepthStencilView* pOldDSV_ = nullptr;



};

