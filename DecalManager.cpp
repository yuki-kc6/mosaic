#include "DecalManager.h"
#include "Decal.h"
#include "Engine/Texture.h"

namespace
{
	constexpr int mosaic_num = 16;//デカール最大値

	DecalBox decalData[mosaic_num];

}

DecalManager::DecalManager(GameObject* parent)
	: GameObject(parent, "DecalManager")
{
}

void DecalManager::InitConstantBuffer(int decalCount)
{
	//デカール用コンスタントバッファの作成
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(DecalBox) * decalCount;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	Direct3D::pDevice_->CreateBuffer(&desc, NULL, &pDecalCB_);

}

void DecalManager::Initialize()
{
	
	//コンスタントバッファ用のメモリ領域を確保(CPU/GPU両方から見れている)
	this->InitConstantBuffer(mosaic_num);
	//とりあえずCPU側から見れるようにしておく(実際はGPU側のメモリを見てるわけではなく、
	//キャッシュコヒーレンシによって連動しているCPU側のメモリを見ている)

	pDecalTexcture_ = new Texture();
	pDecalTexcture_->Load("decal.png");

	D3D11_MAPPED_SUBRESOURCE mapped;
	Direct3D::pContext_->Map(pDecalCB_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	auto decalD = (DecalBox*)mapped.pData;
	Direct3D::pContext_->Unmap(pDecalCB_, 0);

	//初期化
	for (int i = 0; i < mosaic_num; ++i) {
		decalData[i] = {};
	}
	//UpdateShaderConstantBuffer(decalDataH);
}

void DecalManager::Update()
{

	D3D11_MAPPED_SUBRESOURCE mapped;
	Direct3D::pContext_->Map(pDecalCB_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	memcpy(mapped.pData, decalData, sizeof(decalData));
	Direct3D::pContext_->Unmap(pDecalCB_, 0);
}

void DecalManager::Draw()
{
	Direct3D::pContext_->PSSetShaderResources(5, 1, &pTextureSRV_);
	Direct3D::pContext_->PSSetSamplers(5, 1, &pSampleLinear_);
}

void DecalManager::Release()
{

	delete pDecalTexcture_;
}

void DecalManager::AddMosaic(XMFLOAT3 hitPos,XMVECTOR vec )
{
	Decal* decal = Instantiate<Decal>(this);
	XMVECTOR up_vec = XMVectorSet(0, 1, 0, 0);
	decal->SetPosition(hitPos);
	XMVECTOR right =XMVector3Normalize(XMVector3Cross(up_vec, vec));
	XMVECTOR up	   =XMVector3Normalize(XMVector3Cross(vec, right));
	decal->SetAxis({ vec,right,up});
	decal->SetScale(20, 20, 20);
	boxes.push_back(decal);
	if (boxes.size() > mosaic_num) {
		boxes.erase(boxes.begin());
	}

	auto idx = boxes.size() - 1;
	auto& box = boxes.back();
	decalData[idx].enable = 1;
	decalData[idx].pos = box->GetPosition();
	XMStoreFloat3(&decalData[idx].right, box->GetAxis().right);
	XMStoreFloat3(&decalData[idx].up, box->GetAxis().up);
	XMStoreFloat3(&decalData[idx].front, box->GetAxis().front);
	decalData[idx].width = box->GetScale().x;
	decalData[idx].height = box->GetScale().y;
	decalData[idx].depth = box->GetScale().z;

	D3D11_MAPPED_SUBRESOURCE mapped;
	Direct3D::pContext_->Map(pDecalCB_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	auto decalData = (DecalBox*)mapped.pData;
	Direct3D::pContext_->Unmap(pDecalCB_, 0);
}
