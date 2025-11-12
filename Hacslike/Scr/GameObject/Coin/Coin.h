#pragma once
#include "../GameObject.h"
#include"../../Manager/InputManager.h"
#include <memory>
#include <DxLib.h>
#include<vector>

class Player;

class Coin : public GameObject
{
public://静的メンバ変数
	static std::vector<std::unique_ptr<Coin>> coinPool; //オブジェクトプール
	static Coin* pInstance;

public: //メンバ変数
	int  value;				 //コインの値
	int coinModelHandle;	 //コインのモデルハンドル
	int coinValue;           //コインの値（スキルによって値が変更するもの）
	int deleteTime;			 //コインが出現してから消える時間
	float startTime;           //出現時にセットする時間
	int maxCoin;			 //出現するコインの上限
	int currentCoin;		 //現在出現してるコインの数
	bool active;

public: //コンストラクタとデストラクタ
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Coin(VECTOR _pos = VZero, std::string _tag = "", int _value = 0, int _coinValue = 0, int _deleteTime = 20, int _startTime = 0, int _maxCoin = 20, int _currentCoin = 0);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Coin();

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
	static Coin* GetInstance();

	/// <summary>
	/// DestroyInstance
	/// 自身のインスタンスを破棄する唯一の手段
	/// </summary>
	static void DestroyInstance();

public: //メンバ変数

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Start() override;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Render() override;

	/// <summary>
	/// コインの生成
	/// </summary>
	static void SpawnCoin(VECTOR _pos = VZero);

	void UpdateMatrix();

	/// <summary>
	/// コインの削除
	/// </summary>
	void RemoveCoin(Coin* target);

	/// <summary>
	/// ステージ上のコインの取得
	/// </summary>
	/// <param name="player"></param>
	void ApplyCoin(Player* player);

	/// <summary>
	/// オブジェクトプール用のアップデート
	/// </summary>
	static void UpdateAll();
	/// <summary>
	/// オブジェクトプール用の描画
	/// </summary>
	static void RenderAll();
public:
	/// <summary>
	/// コインの値のセット
	/// </summary>
	/// <param name="_coinValue"></param>
	/// <returns></returns>
	inline const void SetCoinValue(int _coinValue) { coinValue = _coinValue; }

	/// <summary>
	/// コインの値の取得
	/// </summary>
	/// <returns></returns>
	inline const int GetCoinValue() { return coinValue; }

	// 表示フラグの変更
	inline void SetActive(bool _a) { active = _a; }

	const std::vector<std::unique_ptr<Coin>>& GetCoin() const { return coinPool; }

public:
	// 平行移動行列を作る関数
	inline MATRIX MGetTrans(float x, float y, float z)
	{
		MATRIX m;

		// 単位行列を作る
		m = MGetIdent();

		// 平行移動成分を代入
		m.m[3][0] = x;
		m.m[3][1] = y;
		m.m[3][2] = z;

		return m;
	}

	// VECTOR 版のオーバーロード
	inline MATRIX MGetTrans(VECTOR pos)
	{
		return MGetTrans(pos.x, pos.y, pos.z);
	}
};
