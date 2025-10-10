#pragma once
#include <string>
#include <list>
#include <map>
#include <DxLib.h>

class EffectManager {
#pragma region シングルトンのデータ構造
private:	// 静的メンバ変数
	static EffectManager* pInstance;	// 自身のインスタンスのアドレスを格納


private:	// コンストラクタとデストラクタ
	/*
	 * @brief	コンストラクタ
	 * @tip		外部で生成されないようにアクセス指定子をprivateにする
	 */
	EffectManager();

	/*
	 * @brief	デストラクタ
	 */
	~EffectManager();

public:	//コピーと譲渡禁止
	EffectManager(const EffectManager&) = delete;
	EffectManager& operator = (const EffectManager&) = delete;
	EffectManager(EffectManager&&) = delete;
	EffectManager& operator = (EffectManager&&) = delete;

private:	// 静的メンバ関数
	/*
	 * @function	CreateInstance
	 * @brief		自信のインスタンスを生成する
	 */
	static void CreateInstance();

public:	// 静的メンバ関数
	/*
	 * @function	GetInstance
	 * @brief		自信のインスタンスを取得する唯一の手段
	 * @return		InputManager*	自身のインスタンスのアドレス
	 */
	static EffectManager* GetInstance();

	/*
	 * @function	DestroyInstance
	 * @brief		自信のインスタンスを破棄する唯一の手段
	 */
	static void DestroyInstance();
#pragma endregion

private:
	std::map<std::string, int> effectResourceMap;	// リソースの連想配列
	std::list<class Effect*> pEffectList;		// エフェクトの一元管理

public:
	/*
	 * @function Load
	 *  @brief		エフェクトの読み込み
	 *  @param[in]	std::string _filepath
	 *  @param[in]	std::string _name
	 *  @param[in]	float _magnification = 1.0f
	 */
	void Load(std::string _filePath, std::string _name, float _magnification = 1.0f);

	/*
	 * @function	Instantiate
	 * @brief		エフェクトの発生
	 * @param[in]	std::string _name
	 * @param[in]	VECTOR _pos
	 * @return		Effect*
	*/
	class Effect* Instantiate(std::string _name, VECTOR _pos);

	void Update();

	void Render();

};

