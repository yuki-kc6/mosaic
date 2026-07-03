#include "PaintObject.h"
#include "Engine/Direct3D.h"
#include "Engine/FbxParts.h"
#include "Engine/VFX.h"
#include "Engine/Global.h"

namespace
{
	constexpr int TEXTURE_SIZE = 512;//モザイクのテクスチャサイズ
	constexpr int DEFAULT_BRUSH_SIZE = 512;//デフォルトのブラシの大きさ

	constexpr float RENDER_NEAR = 0.1f;
	constexpr float RENDER_FAR = 1000.0f;
	constexpr int RENDER_SAMPLE_COUNT = 1;

	constexpr int GRID_DIVISION = 4;
	constexpr float COMPLETE_SCORE = 0.7f;

	constexpr float PAINT_EFFECT_DIRECTION_RANDOM = 60.0f;
	constexpr float PAINT_EFFECT_SPEED = 0.5f;
	constexpr float PAINT_EFFECT_SPEED_RANDOM = 0.5f;
	constexpr float PAINT_EFFECT_ACCEL = 0.9f;
	constexpr float PAINT_EFFECT_GRAVITY = 0.05f;

	constexpr float PAINT_EFFECT_SIZE = 1.0f;
	constexpr float PAINT_EFFECT_SIZE_RANDOM = 0.5f;
	constexpr float PAINT_EFFECT_SCALE = 0.95f;

	constexpr float PAINT_EFFECT_LIFETIME = 20.0f;
	constexpr int PAINT_EFFECT_NUMBER = 10;
	constexpr int PAINT_EFFECT_DELAY = 0;
	constexpr XMFLOAT4 PAINT_EFFECT_DELTA_COLOR = { 0,0,0,-0.05f };
}


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
	:GameObject(parent,name), mosaicRT(nullptr),textureSize(TEXTURE_SIZE),
	isAllPainted(false),score_(0),
	paintedCount(0),isSensitive(false),isOK(false),brushSize(DEFAULT_BRUSH_SIZE)
{
	paintObjectList.push_back(this);
	mosaicRT = new RenderTexture();
	mosaicRT->Initialize(Direct3D::pDevice_, textureSize, textureSize, RENDER_NEAR, RENDER_FAR, RENDER_SAMPLE_COUNT);
	mosaicRT->ClearRenderTarget(Direct3D::pContext_, 0.0, 0.0, 0.0, 1);

	gridSize = textureSize / GRID_DIVISION;

	isPaint = std::vector<std::vector<bool>>(gridSize, std::vector<bool>(gridSize, false));
}

PaintObject::~PaintObject()
{
	paintObjectList.remove(this);

	SAFE_DELETE(mosaicRT);
}

void PaintObject::PaintMosaic(XMFLOAT2 uv,XMFLOAT3 hitPos,XMFLOAT3 normal)
{
	this->CalculateScore(uv, brushSize);
	score_= (float)paintedCount / (float)((gridSize * gridSize));

	if (score_> COMPLETE_SCORE)
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
	data.directionRnd = XMFLOAT3(PAINT_EFFECT_DIRECTION_RANDOM, PAINT_EFFECT_DIRECTION_RANDOM, PAINT_EFFECT_DIRECTION_RANDOM); // 広めに散らばる

	data.speed = PAINT_EFFECT_SPEED;      // 遅めに
	data.speedRnd = PAINT_EFFECT_SPEED_RANDOM;   // 速度をバラバラに
	data.accel = PAINT_EFFECT_ACCEL;      // 徐々に減速
	data.gravity = PAINT_EFFECT_GRAVITY;   // 重力で落ちる

	data.size = XMFLOAT2(PAINT_EFFECT_SIZE, PAINT_EFFECT_SIZE);  // 小さめ
	data.sizeRnd = XMFLOAT2(PAINT_EFFECT_SIZE_RANDOM, PAINT_EFFECT_SIZE_RANDOM);
	data.scale = XMFLOAT2(PAINT_EFFECT_SCALE, PAINT_EFFECT_SCALE); // 徐々に小さく

	data.lifeTime = PAINT_EFFECT_LIFETIME;   
	data.number = PAINT_EFFECT_NUMBER;       // 一度に10粒
	data.delay = PAINT_EFFECT_DELAY;         // 1回だけ発生

	data.deltaColor = PAINT_EFFECT_DELTA_COLOR; // 徐々に透明に

	VFX::Start(data);
}

