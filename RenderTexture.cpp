#include "RenderTexture.h"
#include "Engine/Direct3D.h"
#include "Engine/Global.h"

RenderTexture::RenderTexture()
{
    pRenderTargetTexture_ = 0;
    pRenderTargetTexture_ = 0;
    pShaderResourceView_ = 0;
    pDepthStencilBuffer_ = 0;
    pDepthStencilView_ = 0;
}

RenderTexture::RenderTexture(const RenderTexture&)
{
}

RenderTexture::~RenderTexture()
{
}

bool RenderTexture::Initialize(ID3D11Device* texture, int texWidth, int texHeight, float screenDepth, float screenNear, int format)
{
    D3D11_TEXTURE2D_DESC textureDesc;
    HRESULT result;
    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
    D3D11_TEXTURE2D_DESC depthBufferDesc;
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

    //テクスチャの幅と高さを保存
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
	result = Direct3D::pDevice_->CreateTexture2D(&textureDesc, NULL, &pRenderTargetTexture_);
    if (FAILED(result))
    {
        return false;
    }

	// レンダーターゲットビューの設定
    renderTargetViewDesc.Format = textureDesc.Format;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;

    //レンダーターゲットビューの作成
    result = Direct3D::pDevice_->CreateRenderTargetView(pRenderTargetTexture_, &renderTargetViewDesc, &pRenderTargetView_);
    if (FAILED(result))
    {
        return false;
    }
    
    // シェーダーリソースビューの設定
    shaderResourceViewDesc.Format = textureDesc.Format;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;

    //シェーダーリソースビューの作成
    result = Direct3D::pDevice_->CreateShaderResourceView(pRenderTargetTexture_, &shaderResourceViewDesc, &pShaderResourceView_);
    if (FAILED(result))
    {
        return false;
    }

    //深度バッファを初期化
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

    //深度バッファの設定
    depthBufferDesc.Width = textureWidth;
    depthBufferDesc.Height = textureHeight;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;


    //深度バッファの作成
    result = Direct3D::pDevice_->CreateTexture2D(&depthBufferDesc, NULL, &pDepthStencilBuffer_);
    if (FAILED(result))
    {
        return false;
    }

    //深度ステンシルビューの初期化
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

    //深度ステンシルビューの設定
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    //深度ステンシルビューの作成
    result = Direct3D::pDevice_->CreateDepthStencilView(pDepthStencilBuffer_, &depthStencilViewDesc, &pDepthStencilView_);
    if (FAILED(result))
    {
        return false;
    }

    //ビューポートの設定
    viewPort_.Width = (float)textureWidth;
    viewPort_.Height = (float)textureHeight;
    viewPort_.MinDepth = 0.0f;
    viewPort_.MaxDepth = 1.0f;
    viewPort_.TopLeftX = 0;
    viewPort_.TopLeftY = 0;

    //プロジェクション行列の設定
    projectionMatrix = XMMatrixPerspectiveFovLH((3.141592654f / 4.0f), ((float)textureWidth / (float)textureHeight), screenNear, screenDepth);

    //2D描画のための直交投影行列を作成する
    orthoMatrix = XMMatrixOrthographicLH((float)textureWidth, (float)textureHeight, screenNear, screenDepth);

    return true;

}

void RenderTexture::Shutdown()
{
    if (pDepthStencilView_)
    {   
        pDepthStencilView_->Release();
        pDepthStencilView_ = 0;
    }   
        
    if (pDepthStencilBuffer_)
    {   
        pDepthStencilBuffer_->Release();
        pDepthStencilBuffer_ = 0;
    }   
        
    if (pShaderResourceView_)
    {   
        pShaderResourceView_->Release();
        pShaderResourceView_ = 0;
    }   
        
    if (pRenderTargetView_)
    {   
        pRenderTargetView_->Release();
        pRenderTargetView_ = 0;
    }   
        
    if (pRenderTargetTexture_)
    {   
        pRenderTargetTexture_->Release();
        pRenderTargetTexture_= 0;
    }

    return;
}

void RenderTexture::SetRenderTarget(ID3D11DeviceContext* deviceContext)
{
    // Bind the render target view and depth stencil buffer to the output render pipeline.
     deviceContext->OMSetRenderTargets(1, &pRenderTargetView_, pDepthStencilView_);

    // Set the viewport.
     deviceContext->RSSetViewports(1, &viewPort_);

    return;
}

void RenderTexture::ClearRenderTarget(ID3D11DeviceContext* deviceContext, float red, float green, float blue, float alpha)
{
    float color[4];


    // Setup the color to clear the buffer to.
    color[0] = red;
    color[1] = green;
    color[2] = blue;
    color[3] = alpha;

    // Clear the back buffer.
    deviceContext->ClearRenderTargetView(pRenderTargetView_, color);

    // Clear the depth buffer.
    deviceContext->ClearDepthStencilView(pDepthStencilView_, D3D11_CLEAR_DEPTH, 1.0f, 0);

    return;
}



