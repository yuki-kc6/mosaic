#pragma once
#include "Engine/GameObject.h"
#include "RenderTexture.h"
#include "MosaicPrinter.h"
#include <list>
class PaintObject :
    public GameObject
{
public:
	PaintObject();
	PaintObject(GameObject* parent);
	PaintObject(GameObject* parent, const std::string& name);
	virtual ~PaintObject();
	//GameObjectからそのまま引き継ぐ
	virtual void Initialize(void)override = 0;
	virtual void Update(void)override = 0;
	virtual void Draw() override= 0;
	virtual void Release(void)override = 0;

	//モザイクを塗るための関数
	void PaintMosaic(XMFLOAT2 uv);

	virtual RenderTexture* GetMosaicRT() { return mosaicRT; }

	static const std::list<PaintObject*>& GetPaintObjectList() { return paintObjectList_; }

private:
	RenderTexture* mosaicRT;//オブジェクトごとにRenderTextureを作成
	static std::list<PaintObject*> paintObjectList_;
};

