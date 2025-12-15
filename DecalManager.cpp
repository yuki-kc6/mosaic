#include "DecalManager.h"
#include "Decal.h"
#include "Engine/Texture.h"

namespace
{
	constexpr int mosaic_num = 16;//デカール最大値

}

DecalManager::DecalManager(GameObject* parent)
	: GameObject(parent, "DecalManager")
{
	boxes.clear();
}
 
void DecalManager::InitConstantBuffer(int decalCount)
{
	//デカール用コンスタントバッファの作成
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = sizeof(DecalBox) * decalCount;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	Direct3D::pDevice_->CreateBuffer(&desc, NULL, &pDecalCB_);

}

void DecalManager::Initialize()
{
	pDecalTexture_ = new Texture();
	pDecalTexture_->Load("decal.png");


	//コンスタントバッファ用のメモリ領域を確保(CPU/GPU両方から見れている)
	this->InitConstantBuffer(mosaic_num);
	//とりあえずCPU側から見れるようにしておく(実際はGPU側のメモリを見てるわけではなく、
	//キャッシュコヒーレンシによって連動しているCPU側のメモリを見ている)

	D3D11_MAPPED_SUBRESOURCE mapped;
	Direct3D::pContext_->Map(pDecalCB_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	decalCBuff = (DecalBox*)mapped.pData;
	
	//初期化
	for (int i = 0; i < mosaic_num; ++i) {
		decalCBuff[i] = {};
	}

	memcpy(mapped.pData, decalCBuff, sizeof(DecalBox) * mosaic_num);

	Direct3D::pContext_->Unmap(pDecalCB_, 0);

	


}

void DecalManager::Update()
{
	pTextureSRV_ = pDecalTexture_->GetSRV();
	pSampleLinear_ = pDecalTexture_->GetSampler();
}

void DecalManager::Draw()
{
	Direct3D::pContext_->PSSetShaderResources(1, 1, &pTextureSRV_);
	Direct3D::pContext_->PSSetSamplers(0, 1, &pSampleLinear_);
	Direct3D::pContext_->PSSetConstantBuffers(1, 1, &pDecalCB_);
}

void DecalManager::Release()
{

	delete pDecalTexture_;
}

void DecalManager::AddMosaic(XMFLOAT3 hitPos,XMVECTOR vec )
{
	//ワールドから見た上のベクトル
	XMVECTOR V_UP= XMVectorSet(0, 1, 0, 0);

	Decal* pDecal = Instantiate<Decal>(this);
	pDecal->SetPosition(hitPos);

	XMFLOAT3 front;
	XMStoreFloat3(&front, vec);

	XMVECTOR vRight = XMVector3Normalize(XMVector3Cross(V_UP, vec));
	XMFLOAT3 right;
	XMStoreFloat3(&right, vRight);

	XMVECTOR vUp = XMVector3Normalize(XMVector3Cross(vec, vRight));
	XMFLOAT3 up;
	XMStoreFloat3(&up, vUp);

	pDecal->SetAxis({front,right,up});

	boxes.push_back(pDecal);
	if (boxes.size() > mosaic_num) {//最大値を超えたら最初のデカールは削除
		boxes.pop_front();
	}
	//定数バッファは最後のデカールのみ更新する
	auto idx = boxes.size() - 1;
	const auto& box = boxes.back();
	decalCBuff[idx].enable = 1;
	decalCBuff[idx].pos = box->GetPosition();

	decalCBuff[idx].front = box->GetAxis().front;
	decalCBuff[idx].right = box->GetAxis().right;
	decalCBuff[idx].up = box->GetAxis().up;

	decalCBuff[idx].width = box->GetScale().x;
	decalCBuff[idx].height = box->GetScale().y;
	decalCBuff[idx].depth = box->GetScale().z;
	
	D3D11_MAPPED_SUBRESOURCE mapped;
	Direct3D::pContext_->Map(pDecalCB_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	memcpy(mapped.pData, decalCBuff, sizeof(DecalBox) * mosaic_num);

	Direct3D::pContext_->Unmap(pDecalCB_, 0);
}



