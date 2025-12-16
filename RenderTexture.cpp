#include "RenderTexture.h"
#include "Engine/Direct3D.h"
#include "Engine/Global.h"

bool RenderTexture::Create(int w,int h)
{
    D3D11_TEXTURE2D_DESC desc{};
    desc.Width = w;
    desc.Height = h;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

    Direct3D::pDevice_->CreateTexture2D(&desc, nullptr, &pTexure_);
    Direct3D::pDevice_->CreateRenderTargetView(pTexure_, nullptr, &pRenderTextureRTV_);
    Direct3D::pDevice_->CreateShaderResourceView(pTexure_, nullptr, &pRenderTextureSRV_);


	return true;
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
