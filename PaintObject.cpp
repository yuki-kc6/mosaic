#include "PaintObject.h"
#include "Engine/Direct3D.h"
#include "Engine/FbxParts.h"

std::list<PaintObject*> PaintObject::paintObjectList;

PaintObject::PaintObject()
	:GameObject(), mosaicRT(nullptr)
{

} 

PaintObject::PaintObject(GameObject* parent)
	:GameObject(parent), mosaicRT(nullptr)
{
	
}

PaintObject::PaintObject(GameObject* parent, const std::string& name)
	:GameObject(parent,name), mosaicRT(nullptr),textureSize(512),
	isAllPaint(0),score_(0),
	paintedCount(0),isSensitive(false),isOK(false),brushSize(0.05f)
{
	paintObjectList.push_back(this);
	mosaicRT = new RenderTexture();
	mosaicRT->Initialize(Direct3D::pDevice_, textureSize, textureSize, 0.1f, 1000.0f, 1);
	mosaicRT->ClearRenderTarget(Direct3D::pContext_, 0.0, 0.0, 0.0, 1);

	gridSize = textureSize / 4;

	isPaint = std::vector<std::vector<bool>>(gridSize, std::vector<bool>(gridSize, false));
}

void PaintObject::PaintMosaic(XMFLOAT2 uv)
{
	this->CalculateScore(uv, brushSize);
	score_= (float)paintedCount / (float)((gridSize * gridSize));

	if (score_> 0.3)
	{
		isAllPaint = 1.0;
		isOK = true;
	}

	MosaicPrinter::BeginPaint(mosaicRT);
	MosaicPrinter::Paint(mosaicRT, uv, brushSize, isAllPaint);
	MosaicPrinter::EndPaint();
	
}

void PaintObject::CalculateScore(XMFLOAT2 uv,float brush)
{
	int centerX = (int)(uv.x * (gridSize-1));
	int centerY = (int)((1.0f - uv.y) * (gridSize - 1));

	int brushPixel = (int)(brush * gridSize);

	int radiusSq = brushPixel * brushPixel;

	for(int x = -brushPixel; x <= brushPixel; x++)
	{
		for (int y = -brushPixel; y <= brushPixel; y++)
		{
			if (x * x + y * y > radiusSq) continue;

			int px = centerX + x;
			int py = centerY + y;

			if (px<0 || px>gridSize-1)continue;
			if (py<0 || py>gridSize-1)continue;
			
			if (!isPaint[py][px])
			{
				isPaint[py][px] = true;
				paintedCount++;
			}
		}
	}
}
