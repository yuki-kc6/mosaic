#include "PaintObject.h"
#include "Engine/Direct3D.h"
#include "Engine/FbxParts.h"
#include "Engine/VFX.h"
#include "Engine/Global.h"

namespace
{
	constexpr int TEXTURE_SIZE = 512;//モザイクのテクスチャサイズ
	constexpr int DEFAULT_BRUSH_SIZE = 512;//デフォルトのブラシの大きさ

	constexpr float RENDER_NEAR = 0.1f;//レンダリングの近距離
	constexpr float RENDER_FAR = 1000.0f;//レンダリングの遠距離
	constexpr int RENDER_SAMPLE_COUNT = 1;//レンダリングのサンプル数

	constexpr int GRID_DIVISION = 4;//モザイクの分割数
	constexpr float COMPLETE_SCORE = 0.7f;//塗り終わったと判定するスコア

	constexpr float PAINT_EFFECT_DIRECTION_RANDOM = 60.0f;// 法線方向に飛ぶが、少し散らばるようにする
	constexpr float PAINT_EFFECT_SPEED = 0.5f;// 遅めに
	constexpr float PAINT_EFFECT_SPEED_RANDOM = 0.5f;// 速度をバラバラに
	constexpr float PAINT_EFFECT_ACCEL = 0.9f;// 徐々に減速
	constexpr float PAINT_EFFECT_GRAVITY = 0.05f;// 重力で落ちる
	constexpr float PAINT_EFFECT_SIZE = 1.0f;//そのまま
	constexpr float PAINT_EFFECT_SIZE_RANDOM = 0.5f;// 少しバラバラに
	constexpr float PAINT_EFFECT_SCALE = 0.95f;// 徐々に小さく
	constexpr float PAINT_EFFECT_LIFETIME = 20.0f;// 20フレームで消える
	constexpr int PAINT_EFFECT_NUMBER = 10;// 一度に10粒
	constexpr int PAINT_EFFECT_DELAY = 0;// 1回だけ発生
	constexpr XMFLOAT4 PAINT_EFFECT_DELTA_COLOR = { 0,0,0,-0.05f };// 徐々に透明に
}


std::list<PaintObject*> PaintObject::paintObjectList;//静的変数の初期化

PaintObject::PaintObject()
	:GameObject(), mosaicRT(nullptr), textureSize_(TEXTURE_SIZE), paintRate_(0), paintedCount_(0), gridSize_(0), paintAll_(0),
	isAllPainted_(false), isSensitive_(false), brushSize_(DEFAULT_BRUSH_SIZE)
{

} 

PaintObject::PaintObject(GameObject* parent)
	:GameObject(parent), mosaicRT(nullptr), textureSize_(TEXTURE_SIZE), paintRate_(0), paintedCount_(0), gridSize_(0), paintAll_(0),
	isAllPainted_(false), isSensitive_(false), brushSize_(DEFAULT_BRUSH_SIZE)
{
	
}

PaintObject::PaintObject(GameObject* parent, const std::string& name)
	:GameObject(parent, name), mosaicRT(nullptr), textureSize_(TEXTURE_SIZE), paintRate_(0), paintedCount_(0), gridSize_(0), paintAll_(0),
	isAllPainted_(false),isSensitive_(false),brushSize_(DEFAULT_BRUSH_SIZE)
{
	paintObjectList.push_back(this);//塗れるオブジェクトのリストに追加
	mosaicRT = new RenderTexture();//RenderTextureの作成
	mosaicRT->Initialize(Direct3D::pDevice_, textureSize_, textureSize_, RENDER_NEAR, RENDER_FAR, RENDER_SAMPLE_COUNT);//RenderTextureの初期化
	mosaicRT->ClearRenderTarget(Direct3D::pContext_, 0.0, 0.0, 0.0, 1);//RenderTextureを黒で初期化

	gridSize_ = textureSize_ / GRID_DIVISION;//塗られた割合の計算のためのグリッドのサイズを計算

	isPaint_ = std::vector<std::vector<bool>>(gridSize_, std::vector<bool>(gridSize_, false));//グリッドを用いてisPaintの初期化
}

PaintObject::~PaintObject()
{
	paintObjectList.remove(this);//塗れるオブジェクトのリストから削除

	SAFE_DELETE(mosaicRT);//RenderTextureの解放
}

void PaintObject::PaintMosaic(XMFLOAT2 uv,XMFLOAT3 hitPos,XMFLOAT3 normal)
{
	this->CountPaintedPixels(uv, brushSize_);//塗られたためカウントの更新
	paintRate_ = (float)paintedCount_ / (float)((gridSize_ * gridSize_));//塗られた割合を計算

	// 一定以上塗られたら全て塗った状態にする
	if (paintRate_> COMPLETE_SCORE)
	{
		paintAll_ = 1.0;
		isAllPainted_ = true;
	}
	
	PaintEffect(hitPos, normal);//塗った場所にエフェクトを発生させる
	// モザイクをシェーダーで塗る
	MosaicPrinter::BeginPaint(mosaicRT);
	MosaicPrinter::Paint(mosaicRT, uv, brushSize_, paintAll_);
	MosaicPrinter::EndPaint();
	
}

void PaintObject::CountPaintedPixels(XMFLOAT2 uv,float brush)
{
	// UV座標をグリッド座標に変換
	int centerX = (int)(uv.x * (gridSize_-1));
	int centerY = (int)((1.0f - uv.y) * (gridSize_ - 1));

	// ブラシの半径をグリッド座標に変換
	int brushPixel = (int)(brush * gridSize_);

	// ブラシの半径の二乗を計算（円形の範囲を判定するため）
	int radiusSq = brushPixel * brushPixel;

	// ブラシの範囲内のグリッドを走査
	for(int x = -brushPixel; x <= brushPixel; x++)
	{
		for (int y = -brushPixel; y <= brushPixel; y++)
		{
			// 円形のブラシ範囲外は無視
			if (x * x + y * y > radiusSq) continue;

			int px = centerX + x;
			int py = centerY + y;

			// グリッドの範囲外は無視
			if (px<0 || px>gridSize_-1)continue;
			if (py<0 || py>gridSize_-1)continue;
			
			//初めて塗られたなら記録
			if (!isPaint_[py][px])
			{
				isPaint_[py][px] = true;
				paintedCount_++;
			}
		}
	}
}

void PaintObject::PaintEffect(XMFLOAT3 hitPos,XMFLOAT3 normal)
{
	// エフェクトのデータを設定
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

