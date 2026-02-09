#pragma once
#include <vector>
#include <list>
#include "../Character/Character.h"
#include <iostream>
#include <memory>
#include "../../Component/Singleton.h"

class ItemBase;

enum ShopState {
	Invalid = 1,
	Menu,
	Buy,
	Sell
};

struct BuyItemData {
	ItemBase* item;
	// 価格
	int prace;

	VECTOR imagePos;
	VECTOR namePos;
	VECTOR pracePos;

	// 売ったか
	bool isSell = false;

	void SetPrace(int value) { prace = value * 1.3f; }
};

class ItemShop : public Character , public Singleton<ItemShop> {

public:
	ItemShop();
	~ItemShop();

private:
	// アイテムのID
	const std::string potion_small_ID = "Potion_Small";
	const std::string potion_middle_ID = "Potion_Middle";
	const std::string potion_large_ID = "Potion_Large";
	const std::string potion_attack_ID = "AttactPotion";
	const std::string potion_defence_ID = "DefensePotion";
	const std::string grenade_ID = "Grenade";
	const std::string sword_Iron_ID = "Sword_Iron";
	const std::string axe_ID = "Axe";
	const std::string great_Sword_ID = "Greatsword";
	const std::string spear_ID = "Spear";

	// アイテムの種類数
	const int potionChoiceNum = 4;
	const int weaponChoiceNum = 1;
	const int saleChoiceNum = 1;


	unsigned int coinColor = GetColor(255, 215, 0);
	// IDのリスト
	std::vector<std::string> potionIDList;
	std::vector<std::string> weaponIDList;
	std::vector<std::string> allItemIDList;

	// 店頭に並ぶアイテムのリスト
	std::vector<BuyItemData> buyItem;
	std::vector<unique_ptr<ItemBase>> buyItemData;

	// ショップの状態
	ShopState state;

	// ショップのオブジェクトを触ってるか
	bool isTouch;

	// アイテムの抽選をしたか
	bool isChoice;

	// ショップメニューでどっちを選択しているか(0 : Buy,1 : Sell)
	int selectCommand = 0;

	// アイテムのアイコンのオフセット
	VECTOR buyItemIconOffSet = VGet(WINDOW_WIDTH / 4, WINDOW_HEIGHT / 2.5f, 0);

	int selectBuyCommand = 0;
private:
	void ChoiceItem(std::vector<std::string> _itemIDList ,int _choiceCount);

	void SetPrace();
	void MathDrawPos(BuyItemData* data,VECTOR drawOffset);
	void DrawBuyItem(BuyItemData data);


	void DeadExecute() override;
	void OpenExecute();
public:
	void Start() override;
	void Update() override;
	void Render() override;
	void OnTriggerEnter(Collider* _pOhter) override;
	void OnTriggerExit(Collider* _pOhter) override;
	void Setup();
	ShopState GetState() { return state; }
};

