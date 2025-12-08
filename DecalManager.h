#pragma once
#include "Engine/global.h"
#include "Engine/GameObject.h"
#include <vector>
//デカール直方体
//定数バッファでHLSLに渡すときは16byteパッキングされてしまうため
//このように16バイト区切りになるように並べている
struct DecalBox {
	XMFLOAT3 pos;//デカール
	int enable;//有効フラグ

	XMFLOAT3 right;//右ベクトル
	float width;//幅

	XMFLOAT3 up;//上ベクトル
	float height;//高さ

	XMFLOAT3 front;//前ベクトル
	float depth;//奥行
};





//-----------------------------------------------------------
//デカールを管理するオブジェクト
//-----------------------------------------------------------
class DecalManager : public GameObject
{
public:
	ID3D11Buffer* pDecalCB_ = nullptr;
	//コンストラクタ
	DecalManager(GameObject* parent);
	void InitConstantBuffer(int decalCount);

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void AddMosaic(XMFLOAT3 hitPos, XMVECTOR vec);

private:
	std::vector<Transform> boxes;

};