#pragma once



//
//トランジションを管理するクラス
//
struct TRANSITION_CONSTANT_BUFFER
{
	float progress;
	float padding[3];
};


namespace Transition
{
	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw();
	
	//画面がフェードアウトする
	void StartFadeOut();

	//画面がフェードインする
	void StartFadeIn();
	
	//始まっているか
	void IsPlayer();

	//終わったか
	void IsFinished();

	//解放
	void Release();
}