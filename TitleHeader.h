#pragma once
#include "Engine/GameObject.h"
//タイトル画像用のクラス
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
    int hPic_;//PushtoStart画像用ハンドル
    int hTitle_;//タイトル画像用ハンドル
};

