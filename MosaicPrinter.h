#pragma once
#include "RenderTexture.h"
#include "Engine/GameObject.h"

class MosaicPrinter :
	public GameObject
{
public:
	MosaicPrinter();
	~MosaicPrinter();
	void DrawMosaic(RenderTexture* target,XMFLOAT2 uv,float radius);


};

