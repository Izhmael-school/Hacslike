#include "EffectManager.h"
#include "EffekseerForDXLib.h"
#include"../GameObject/Effect/Effect.h"

EffectManager::EffectManager()
	:effectResourceMap()
	, pEffectList() {

}

EffectManager::~EffectManager() {
	for (auto itr : effectResourceMap) {
		DeleteEffekseerEffect(itr.second);
	}
	effectResourceMap.clear();

	for (auto pEffe : pEffectList) {
		if (pEffe != nullptr) {
			delete pEffe;
			pEffe = nullptr;
		}
	}
	pEffectList.clear();

}

/*
 * @function Load
 *  @brief		エフェクトの読み込み
 *  @param[in]	std::string _filepath
 *  @param[in]	std::string _name
 *  @param[in]	float _magnification = 1.0f
 */
void EffectManager::Load(std::string _filePath, std::string _name, float _magnification) {
	int res = LoadEffekseerEffect(_filePath.c_str(), _magnification);

	// リソースの管理
#if 0
	effectResourceMap[_filePath.c_str()] = res;
#else
	auto itr = effectResourceMap.find(_filePath.c_str());

	if (itr == effectResourceMap.end()) {
		// 登録
		effectResourceMap.emplace(_name.c_str(), res);
	}
#endif
}

/*
 * @function	Instantiate
 * @brief		エフェクトの発生
 * @param[in]	std::string _name
 * @param[in]	VECTOR _pos
 * @return		Effect*
 */
Effect* EffectManager::Instantiate(std::string _name, VECTOR _pos) {
	Effect* pEffect = new Effect(effectResourceMap[_name]);
	pEffect->SetPosition(_pos);
	pEffectList.push_back(pEffect);
	return pEffect;

	return pEffect;
}

void EffectManager::Update() {
	for (auto pEffe : pEffectList) {
		if (pEffe == nullptr || !pEffe->IsVisible())
			continue;

		pEffe->Update();
	}

	// STLの要素を削除
	// std::erase_if( コンテナ, ラムダ)	C++20～ ここじゃ使えん
	// isVisible = false のものを消す
	std::erase_if(pEffectList, [](Effect* _pE) {return !_pE->IsVisible(); });

	UpdateEffekseer3D();
}

void EffectManager::Render() {
	for (auto pEffe : pEffectList) {
		if (pEffe == nullptr || !pEffe->IsVisible())
			continue;

		pEffe->Render();
	}
	DrawEffekseer3D();
}

