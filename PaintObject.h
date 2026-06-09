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
	PaintObject();
	PaintObject(GameObject* parent);
	PaintObject(GameObject* parent, const std::string& name);
	//virtual ~PaintObject()override;
	//GameObjectからそのまま引き継ぐ
	virtual void Initialize(void)override = 0;
	virtual void Update(void)override = 0;
	virtual void Draw() override= 0;
	virtual void Release(void)override = 0;

	//モザイクを塗るための関数
	void PaintMosaic(XMFLOAT2 uv, XMFLOAT3 hitPos, XMFLOAT3 normal);

	void CalculateScore(XMFLOAT2 uv, float brushSize);

	float getScore() { return score_; }

	virtual RenderTexture* GetMosaicRT() { return mosaicRT; }

	static const std::list<PaintObject*>& GetPaintObjectList() { return paintObjectList; }
	
	bool IsSensitive() { return isSensitive; }
	void SetSensitive(bool sen) { isSensitive = sen; }

	bool IsClear() { return isOK; }

	bool IsAllPainted() { return isAllPainted; }

	void SetBrushSize(float size) { brushSize = size; }

private:
	void PaintEffect(XMFLOAT3 hitPos,XMFLOAT3 normal);
	RenderTexture* mosaicRT;//オブジェクトごとにRenderTextureを作成
	static std::list<PaintObject*> paintObjectList;
	int textureSize;
	float score_;
	int paintedCount;
	int gridSize;
	std::vector<std::vector<bool>> isPaint;
	float paintAll;//シェーダーに塗られたら1.0fを渡すための変数

	bool isAllPainted;//全て塗られたかどうか

	bool isSensitive;
	bool isOK;

	float brushSize;


	
};


