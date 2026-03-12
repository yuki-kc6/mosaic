#pragma once
#include "Engine/GameObject.h"
class ResultHeader :
    public GameObject
{
public:
    ResultHeader(GameObject* parent);
    ~ResultHeader();
    void Initialize()override;
    void Update()override;
    void Draw()override;
    void Release()override;
private:
    int hPic_;
};

