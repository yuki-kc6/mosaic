#include "DecalManager.h"


namespace
{
	constexpr int mosaic_num = 16;//弾痕デカール最大値

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
	D3D11_MAPPED_SUBRESOURCE mapped;
	Direct3D::pContext_->Map(pDecalCB_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	auto decalCBuff = (DecalBox*)mapped.pData;
	Direct3D::pContext_->Unmap(pDecalCB_, 0);
	//初期化
	for (int i = 0; i < mosaic_num; ++i) {
		decalCBuff[i] = {};
	}
	//UpdateShaderConstantBuffer(decalCBuffH);
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
}

void DecalManager::Release()
{
}

void DecalManager::AddMosaic(XMFLOAT3 hitPos,XMVECTOR vec )
{
	Transform mBox;
	XMVECTOR up_vec = XMVectorSet(0, 1, 0, 0);
	mBox.position_= hitPos;
	XMVECTOR vFront = vec;
	XMFLOAT3 front;
	XMStoreFloat3(&front, vFront);
	XMVECTOR vRight = XMVector3Normalize(XMVector3Cross(vFront, up_vec));
	XMFLOAT3 right;
	XMStoreFloat3(&right, vRight);
	XMVECTOR vUp = XMVector3Normalize(XMVector3Cross(vFront, vRight));
	XMFLOAT3 up;
	XMStoreFloat3(&up, vUp);

	boxes.push_back();
}
