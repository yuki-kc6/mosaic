#pragma once
#include "Engine/GameObject.h"
#include "RenderTexture.h"
#include "MosaicPrinter.h"
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

	RenderTexture* mosaicRT;//オブジェクトごとにRenderTextureを作成

private:
	

};

