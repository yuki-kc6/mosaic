#include "PaintObject.h"
#include "Engine/Direct3D.h"
#include "Engine/FbxParts.h"
#include "Engine/VFX.h"
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
	isAllPainted(false),score_(0),
	paintedCount(0),isSensitive(false),isOK(false),brushSize(0.05f)
{
	paintObjectList.push_back(this);
	mosaicRT = new RenderTexture();
	mosaicRT->Initialize(Direct3D::pDevice_, textureSize, textureSize, 0.1f, 1000.0f, 1);
	mosaicRT->ClearRenderTarget(Direct3D::pContext_, 0.0, 0.0, 0.0, 1);

	gridSize = textureSize / 4;

	isPaint = std::vector<std::vector<bool>>(gridSize, std::vector<bool>(gridSize, false));
}

void PaintObject::PaintMosaic(XMFLOAT2 uv,XMFLOAT3 hitPos,XMFLOAT3 normal)
{
	this->CalculateScore(uv, brushSize);
	score_= (float)paintedCount / (float)((gridSize * gridSize));

	if (score_> 0.5)
	{
		paintAll = 1.0;
		isAllPainted = true;
		isOK = true;
	}
	PaintEffect(hitPos,normal);
	MosaicPrinter::BeginPaint(mosaicRT);
	MosaicPrinter::Paint(mosaicRT, uv, brushSize, paintAll);
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

void PaintObject::PaintEffect(XMFLOAT3 hitPos,XMFLOAT3 normal)
{
	EmitterData data;
	data.textureFileName = "cloudA.png";
	data.position = hitPos;

	// 法線方向に飛ぶ
	data.direction = normal;
	data.directionRnd = XMFLOAT3(60.0f, 60.0f, 60.0f); // 広めに散らばる

	data.speed = 0.5f;      // 遅めに
	data.speedRnd = 0.5f;   // 速度をバラバラに
	data.accel = 0.9f;      // 徐々に減速
	data.gravity = 0.05f;   // 重力で落ちる

	data.size = XMFLOAT2(1.0f, 1.0f);  // 小さめ
	data.sizeRnd = XMFLOAT2(0.5f, 0.5f);
	data.scale = XMFLOAT2(0.95f, 0.95f); // 徐々に小さく

	data.lifeTime = 20;   
	data.number = 10;       // 一度に10粒
	data.delay = 0;         // 1回だけ発生

	data.deltaColor = XMFLOAT4(0, 0, 0, -0.05f); // 徐々に透明に

	VFX::Start(data);
}

