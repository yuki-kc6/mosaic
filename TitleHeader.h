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
    void ButtonClick(XMFLOAT2 mousePos);//終了ボタン用クリック判定
private:
    int hPic_;//PushtoStart画像用ハンドル
    int hTitle_;//タイトル画像用ハンドル
    int hEndButton_;//終了ボタン用ハンドル
};

