#include "SphereCollider.h"
#include "BoxCollider.h"
#include "Model.h"

//コンストラクタ（当たり判定の作成）
//引数：basePos	当たり判定の中心位置（ゲームオブジェクトの原点から見た位置）
//引数：size	当たり判定のサイズ
SphereCollider::SphereCollider(XMFLOAT3 center, float radius)
{
	center_ = center;
	size_ = XMFLOAT3(radius, radius, radius);
	type_ = COLLIDER_CIRCLE;
	result = { false, XMFLOAT3(0,0,0) };

	//リリース時は判定枠は表示しない
#ifdef _DEBUG
	//テスト表示用判定枠
	hDebugModel_ = Model::Load("DebugCollision/sphereCollider.fbx");
#endif
}

//接触判定
//引数：target	相手の当たり判定
//戻値：接触してればtrue
HitResult SphereCollider::IsHit(Collider* target)
{
	if (target->type_ == COLLIDER_BOX)
	{
		result.isHit = IsHitBoxVsCircle((BoxCollider*)target, this);
		result.pushBack = GetPushBackVecBoxVsCircle((BoxCollider*)target, this);
	}
	else
	{
		result.isHit = IsHitCircleVsCircle((SphereCollider*)target, this);
	}
	return result;
}