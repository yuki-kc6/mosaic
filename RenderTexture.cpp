#include "RenderTexture.h"
#include "Engine/Direct3D.h"
#include "Engine/Global.h"

RenderTexture::RenderTexture()
{

}

RenderTexture::RenderTexture(const RenderTexture&)
{
}

RenderTexture::~RenderTexture()
{
}

bool RenderTexture::Initialize(ID3D11Device*, int texWidth, int texHeight, float screenDepth, float screenNear, int format)
{
    D3D11_TEXTURE2D_DESC textureDesc;
    HRESULT result;
    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
    D3D11_TEXTURE2D_DESC depthBuffferDesc;
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    DXGI_FORMAT textureFormat;

    switch (format)
    {
        case 1:
        {
            textureFormat = DXGI_FORMAT_R8_UNORM;
        }
        default:
        {
            textureFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
            break;
        }
    }

	textureWidth = texWidth;
	textureHeight = texHeight;

    //レンダーターゲットテクスチャを初期化
    ZeroMemory(&textureDesc, sizeof(textureDesc));

    //レンダーターゲットテクスチャの設定
	textureDesc.Width = textureWidth;
    textureDesc.Height = textureHeight;
    textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
    textureDesc.Format = textureFormat;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

    //レンダーターゲットテクスチャの作成
	result = Direct3D::pDevice_->CreateTexture2D(&textureDesc, NULL, &pRenderTexture_);
    if (FAILED(result))
    {
        return false;
    }

    //////////////////////////////////////////////////////////////////////
    
    // Initailze the depth stencil view description.
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

    // Set up the depth stencil view description.
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    // Create the depth stencil view.
    result = device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
    if (FAILED(result))
    {
        return false;
    }

    // Setup the viewport for rendering.
    m_viewport.Width = (float)textureWidth;
    m_viewport.Height = (float)textureHeight;
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;
    m_viewport.TopLeftX = 0;
    m_viewport.TopLeftY = 0;

    // Setup the projection matrix.
    m_projectionMatrix = XMMatrixPerspectiveFovLH((3.141592654f / 4.0f), ((float)textureWidth / (float)textureHeight), screenNear, screenDepth);

    // Create an orthographic projection matrix for 2D rendering.
    m_orthoMatrix = XMMatrixOrthographicLH((float)textureWidth, (float)textureHeight, screenNear, screenDepth);

    return true;

}

void RenderTexture::Shutdown()
{

}

void RenderTexture::SetRenderTarget(ID3D11DeviceContext* target)
{
}

void RenderTexture::ClearRenderTarget(ID3D11DeviceContext* target, float, float, float, float)
{
}

void RenderTexture::Begin()  
{  
   Direct3D::pContext_->OMGetRenderTargets(1, &pOldRTV_, &pOldDSV_);  

   Direct3D::pContext_->OMSetRenderTargets(1, &pRenderTextureRTV_, nullptr);  

   float clear[4] = { 0, 0, 0, 0 };  
   Direct3D::pContext_->ClearRenderTargetView(pRenderTextureRTV_, clear);  
}

void RenderTexture::End()
{
    Direct3D::pContext_->OMSetRenderTargets(1, &pOldRTV_ , pOldDSV_);

    SAFE_RELEASE(pOldRTV_);
    SAFE_RELEASE(pOldDSV_);
}

void RenderTexture::Clear(float r, float g, float b, float a)
{
    float color[4] = { r, g, b, a };
    Direct3D::pContext_->ClearRenderTargetView(pRenderTextureRTV_, color);
}
