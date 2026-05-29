#include "TownNPC.h"

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
	bool isUpdatePainted = false;
};

