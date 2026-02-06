#pragma once
#include "../GameObject.h"
class StartTreasureChest : public GameObject
{
public:
	static StartTreasureChest* pInstance;

private: //メンバ変数
	int TreasureChestModel; //宝箱のモデル
	bool active;

public: //コンストラクタとデストラクタ
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_pos"></param>
	/// <param name="_tag"></param>
	StartTreasureChest(VECTOR _pos = VZero, std::string _tag = "");

	/// <summary>
	/// デストラクタ
	/// </summary>
	~StartTreasureChest();
private://静的メンバ関数
	/// <summary>
	/// CreateInstance
	/// 自信のインスタンスを生成する
	/// </summary>
	static void CreateInstance();

public://静的メンバ関数
	/// <summary>
	/// GetInstance
	/// 自身のインスタンスを取得する唯一の手段
	/// return InputManager*
	/// </summary>
	/// <returns></returns>DINPUT_JOYSTATE
	static StartTreasureChest* GetInstance();

	/// <summary>
	/// DestroyInstance
	/// 自身のインスタンスを破棄する唯一の手段
	/// </summary>
	static void DestroyInstance();
public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Start() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Render() override;

	/// <summary>
	/// 宝箱を開けた時の処理
	/// </summary>
	void OpenTreasureChest();
};

