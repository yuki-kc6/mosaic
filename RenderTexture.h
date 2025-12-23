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

	bool Create(int w, int h);
	void Shutdown();


	void SetRenderTarget(ID3D11DeviceContext* target);
	void ClearRenderTarget(ID3D11DeviceContext* target, float, float, float, float);
	
	ID3D11ShaderResourceView* GetShaderResourceView() { return pRenderTextureSRV_; }	//シェーダーリソースビューの取得

	void GetProjectionMatrix(XMMATRIX);
	void GetOrthoMatrix(XMMATRIX);

	int GetTextureWidth();
	int GetTextureHeight();



	void Begin();
	void End();
	void Clear(float r, float g, float b, float a);

private:
	int textureWidth;
	int textureHeight;


	ID3D11Texture2D* pRenderTexture_=nullptr;
	ID3D11RenderTargetView* pRenderTextureRTV_=nullptr;
	ID3D11ShaderResourceView* pRenderTextureSRV_ = nullptr;


	ID3D11RenderTargetView* pOldRTV_ = nullptr;
	ID3D11DepthStencilView* pOldDSV_ = nullptr;



};

