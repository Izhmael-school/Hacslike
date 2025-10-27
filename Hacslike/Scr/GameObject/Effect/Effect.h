#pragma once
#include "../GameObject.h"
class Effect : public GameObject
{
private: //メンバ変数
	int resourceHandle;  //リソースハンドル
	int playingHandle;	 //再生中ハンドル


public: //コンストラクタとデストラクタ
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_resourceHandle">
	/// int& _resourceHandle
	/// </param>
	Effect(int& _resourceHandle);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Effect();

public: //メンバ関数
	/// <summary>
	/// Start
	/// 初期化処理
	/// </summary>
	void Start() override;

	/// <summary>
	/// Update
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// Render
	/// 描画処理
	/// </summary>
	void Render() override;


public: //セッターとゲッター
};

