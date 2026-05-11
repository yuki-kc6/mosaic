#include "BoxCollider.h"
#include "SphereCollider.h"
#include "Model.h"


//コンストラクタ（当たり判定の作成）
//引数：basePos	当たり判定の中心位置（ゲームオブジェクトの原点から見た位置）
//引数：size	当たり判定のサイズ
BoxCollider::BoxCollider(XMFLOAT3 basePos, XMFLOAT3 size)
{
	center_ = basePos;
	size_ = size;
	type_ = COLLIDER_BOX;
	result = { false, XMFLOAT3(0,0,0) };

	//リリース時は判定枠は表示しない
#ifdef _DEBUG
	//テスト表示用判定枠
	hDebugModel_ = Model::Load("DebugCollision/boxCollider.fbx");
#endif
}

//接触判定
//引数：target	相手の当たり判定
//戻値：接触してればtrue
HitResult BoxCollider::IsHit(Collider* target)
{
	if (target->type_ == COLLIDER_BOX)
		 result.isHit= IsHitBoxVsBox((BoxCollider*)target, this);

	else {
		result.isHit = IsHitBoxVsCircle(this, (SphereCollider*)target);
		result.pushBack = GetPushBackVecBoxVsCircle(this, (SphereCollider*)target);
	}


	return result;
}