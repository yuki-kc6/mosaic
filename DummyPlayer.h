#pragma once
#include "Engine/GameObject.h"

enum PlayerState
{
    IDLE,//待機状態
    CLEAR//クリア時のモーション
};

class DummyPlayer :
    public GameObject
{
	//プレイヤーの見た目のみを表現するダミーオブジェクト
public:
    //コンストラクタ
    DummyPlayer(GameObject* parent);

    //デストラクタ
    ~DummyPlayer();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

    //状態を設定する
	//引数state、IDLE:待機状態、CLEAR:クリア時のモーション
    void SetState(PlayerState state) { state_=state; }

private:
	PlayerState state_;//プレイヤーの状態
	int hClearModel_;//クリア時のモーション用モデルハンドル
};

