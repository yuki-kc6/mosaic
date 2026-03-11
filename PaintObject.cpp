#include "PaintObject.h"
#include "Engine/Direct3D.h"
#include "Engine/FbxParts.h"

std::list<PaintObject*> PaintObject::paintObjectList_;

PaintObject::PaintObject()
	:GameObject(),mosaicRT(nullptr)
{
	//paintObjectList_.push_back(this);
	//mosaicRT = new RenderTexture();
	//mosaicRT->Initialize(Direct3D::pDevice_, 512, 512, 0.1f, 1000.0f, 1);	
	//mosaicRT->ClearRenderTarget(Direct3D::pContext_, 0.0, 0.0, 0.0, 1);
}

PaintObject::PaintObject(GameObject* parent)
	:GameObject(parent), mosaicRT(nullptr)
{
	//paintObjectList_.push_back(this);
	//mosaicRT = new RenderTexture();
	//mosaicRT->Initialize(Direct3D::pDevice_, 512, 512, 0.1f, 1000.0f, 1);
	//mosaicRT->ClearRenderTarget(Direct3D::pContext_, 0.0, 0.0, 0.0, 1);
}

PaintObject::PaintObject(GameObject* parent, const std::string& name)
	:GameObject(parent,name), mosaicRT(nullptr),textureSize(512),isAllPaint(0),score_(0)
{
	paintObjectList_.push_back(this);
	mosaicRT = new RenderTexture();
	mosaicRT->Initialize(Direct3D::pDevice_, textureSize, textureSize, 0.1f, 1000.0f, 1);
	mosaicRT->ClearRenderTarget(Direct3D::pContext_, 0.0, 0.0, 0.0, 1);

	gridSize = textureSize / 4;

	isPaint = std::vector<std::vector<bool>>(gridSize, std::vector<bool>(gridSize, false));
}

void PaintObject::PaintMosaic(XMFLOAT2 uv,float brushSize)
{
	if (float(score_ / (gridSize * gridSize)) > 0.25)
	{
		isAllPaint = 1.0;
	}
	this->CalculateScore(uv, brushSize);
	MosaicPrinter::BeginPaint(mosaicRT);
	MosaicPrinter::Paint(mosaicRT, uv, brushSize, isAllPaint);
	MosaicPrinter::EndPaint();
	
}

void PaintObject::CalculateScore(XMFLOAT2 uv,float brushSize)
{
	int centerX = (int)(uv.x * gridSize);
	int centerY = (int)(uv.y * gridSize);

	int brushPixel = (int)(brushSize * gridSize);

	for(int x = -brushPixel; x <= brushPixel; x++)
	{
		for (int y = -brushPixel; y <= brushPixel; y++)
		{
			if (x * x + y * y > brushPixel * brushPixel) continue;

			int px = centerX + x;
			int py = centerY + y;

			if (px<0 || px>gridSize-1)continue;
			if (py<0 || py>gridSize-1)continue;
			
			if (!isPaint[py][px])
			{
				isPaint[py][px] = true;
d				score_++;
			}
		}
	}
}





