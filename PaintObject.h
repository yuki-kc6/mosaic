#pragma once
#include "Engine/GameObject.h"
#include "RenderTexture.h"
#include "MosaicPrinter.h"
#include <list>
#include <vector>
class PaintObject :
	public GameObject
{
public:
	//コンストラクタ
	PaintObject();
	PaintObject(GameObject* parent);
	PaintObject(GameObject* parent, const std::string& name);
	//デストラクタ
	 ~PaintObject();
	//GameObjectからそのまま引き継ぐ
	virtual void Initialize(void)override = 0;
	virtual void Update(void)override = 0;
	virtual void Draw() override= 0;
	virtual void Release(void)override = 0;

	//モザイクを塗るための関数
	//uv:塗る場所のUV座標
	//hitPos:塗る場所のワールド座標
	//normal:塗る場所の法線ベクトル
	void PaintMosaic(XMFLOAT2 uv, XMFLOAT3 hitPos, XMFLOAT3 normal);

	//オブジェクトにどれだけ塗られたかを計算するための関数
	//uv:塗る場所のUV座標
	//ブラシの半径
	void CountPaintedPixels(XMFLOAT2 uv, float brushSize);

	//を取得するための関数
	float getScore() { return paintRate_; }

	//モザイクのRenderTextureを取得するための関数
	virtual RenderTexture* GetMosaicRT() { return mosaicRT; }

	//塗れるオブジェクトのリストを取得するための関数
	static const std::list<PaintObject*>& GetPaintObjectList() { return paintObjectList; }
	
	//このオブジェクトが現在センシティブかどうかを取得するための関数
	bool IsSensitive() { return isSensitive_; }

	//このオブジェクトがセンシティブかどうかを設定するための関数
	//size:センシティブかどうか　true:センシティブ　false:センシティブでない
	void SetSensitive(bool sen) { isSensitive_ = sen; }

	//このオブジェクトがすべて塗られたかどうかを取得するためお関数
	bool IsAllPainted() { return isAllPainted_; }

	//オブジェクトのブラシサイズを設定するための関数
	//size:ブラシの半径
	void SetBrushSize(float size) { brushSize_= size; }

private:
	void PaintEffect(XMFLOAT3 hitPos,XMFLOAT3 normal);
	RenderTexture* mosaicRT;//オブジェクトごとにRenderTextureを作成
	static std::list<PaintObject*> paintObjectList;//塗れるオブジェクトのリスト
	int textureSize_;//RenderTextureのサイズ
	float paintRate_;//塗られた割合
	int paintedCount_;//どれだけ塗られたかのカウント
	int gridSize_;//塗られた割合の計算のためのグリッドのサイズ

	std::vector<std::vector<bool>> isPaint_;//塗り判定用グリッドの状態を管理する配列.ピクセルだと大きくなるのでグリッドサイズで分割

	float paintAll_;//シェーダーに一定以上塗られたら1.0fを渡し全て塗らせるためfloatで状態を保持する

	bool isAllPainted_;//全て塗られたかどうか

	bool isSensitive_;//このオブジェクトがセンシティブかどうか
	float brushSize_;//ブラシの半径


	
};


