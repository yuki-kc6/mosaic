#include "TownNPC.h"

// 敵キャラクタークラス(TownNPCから拡張)
class Enemy : public TownNPC
{
public:
	Enemy(GameObject* parent);
	~Enemy();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
private:
	int hSound_;
	bool isPainted_;//塗られたかの確認用フラグ
};

