#pragma once
#include "Engine/GameObject.h"
class TitleHeader :public GameObject
{
public:
    TitleHeader(GameObject* parent);
    ~TitleHeader();
    void Initialize()override;
    void Update()override;
    void Draw()override;
    void Release()override;
private:
    int hPic_;
};

