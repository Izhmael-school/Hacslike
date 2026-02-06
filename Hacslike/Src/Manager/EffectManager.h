#pragma once
#include <string>
#include <list>
#include <map>
#include <DxLib.h>
#include "../Component/Singleton.h"

class EffectManager : public Singleton<EffectManager>{

public:	// コンストラクタとデストラクタ
	/*
	 * @brief	コンストラクタ
	 * @tip		外部で生成されないようにアクセス指定子をprivateにする
	 */
	EffectManager();

	/*
	 * @brief	デストラクタ
	 */
	~EffectManager();

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

