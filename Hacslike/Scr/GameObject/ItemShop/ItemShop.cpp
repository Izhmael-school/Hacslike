#include "ItemShop.h"
#include "../../CommonModule.h"
#include "../Item/ItemFactory.h"
#include "../../Manager/InputManager.h"
#include "../Character/Player/Player.h"
#include "../Item/Inventory.h"
#include "../../Component/Collider/Collider.h"
#include "../../GameSystem/GameSystem.h"

ItemShop::ItemShop()
	:isTouch(false)
	, isChoice(false)
	, state(Invalid) {
	Start();
}

ItemShop::~ItemShop() {}

void ItemShop::ChoiceItem(std::vector<std::string> _itemIDList,int _choiceCount) {
	for (int i = 0; i < _choiceCount; i++) {

		if (_itemIDList.size() == 0) return;

		int rand = Random(0, _itemIDList.size() - 1);
		std::string itemID = _itemIDList[rand];
		_itemIDList.erase(_itemIDList.begin() + rand);

		std::unique_ptr<ItemBase> item_ptr = ItemFactory::Instance().CreateItem(itemID);
		ItemBase* item = item_ptr.get();
		buyItemData.push_back(std::move(item_ptr));
		BuyItemData data;
		data.item = item;
		buyItem.push_back(data);
	}
}

void ItemShop::SetPrace() {
	for (BuyItemData& d : buyItem) {
		int minValue = d.item->GetValue();
		int rand = Random(minValue, minValue * 1.5f);
		d.SetPrace(rand);
	}
}

void ItemShop::MathDrawPos(BuyItemData* data, VECTOR drawOffset) {
	// 画像の座標
	VECTOR imagePos = ExpendImageCenterPos(drawOffset, 150, 150);
	data->imagePos = imagePos;
	// 名前の座標
	VECTOR p = VGet(0, 40, 0);
	VECTOR namePos = VSub(imagePos, p);
	namePos.x = drawOffset.x;
	string name = data->item->GetName();
	namePos = StringCenterPos(name.c_str(), MainFont, namePos.x, namePos.y);
	data->namePos = namePos;
	// 価格の座標
	p = VGet(0, 180, 0);
	VECTOR pracePos = VAdd(imagePos, p);
	pracePos.x = drawOffset.x;
	string praceString = MergeString(std::to_string(data->prace), "コイン");
	pracePos = StringCenterPos(praceString.c_str(), MainFont, pracePos.x, pracePos.y);
	data->pracePos = pracePos;
}

void ItemShop::DrawBuyItem(BuyItemData data) {
	if (data.isSell) return;
	// 画像
	DrawExtendGraph(data.imagePos.x, data.imagePos.y, data.imagePos.x + 150, data.imagePos.y + 150, data.item->GetItemImageHandle(), true);
	// 名前
	DrawStringToHandle(data.namePos.x, data.namePos.y, data.item->GetName().c_str(), white, MainFont);
	// 価格
	DrawStringToHandle(data.pracePos.x, data.pracePos.y, MergeString(std::to_string(data.prace), "コイン").c_str(), white, MainFont);
}

void ItemShop::OpenExecute() {
	GameSystem::GetInstance()->SetGameStatus(GameStatus::Stop); // ゲーム停止

	Setup();

	if (!isChoice) {
		isChoice = true;

		ChoiceItem(potionIDList,3);
		ChoiceItem(weaponIDList,2);
		ChoiceItem(allItemIDList,1);
		SetPrace();
	}

	if (buyItem.size() > 0) {
		int col = 3;

		for (int i = 0; i < buyItem.size(); i++) {
			int x = i % col;
			int y = i / col;
			VECTOR pos = VMult(buyItemIconOffSet, VGet(x + 1, y + 1, 0));
			MathDrawPos(&buyItem[i], pos);
		}
	}
}

void ItemShop::Setup() {
	isChoice = false;

	buyItem.clear();
	buyItemData.clear();
}

void ItemShop::Start() {
	potionIDList.push_back(potion_small_ID);
	potionIDList.push_back(potion_middle_ID);
	potionIDList.push_back(potion_large_ID);
	potionIDList.push_back(potion_attack_ID);
	potionIDList.push_back(potion_defence_ID);
	weaponIDList.push_back(grenade_ID);
	weaponIDList.push_back(sword_Iron_ID);
	weaponIDList.push_back(axe_ID);
	weaponIDList.push_back(great_Sword_ID);
	weaponIDList.push_back(spear_ID);

	allItemIDList = MergeVector<std::string>(potionIDList, weaponIDList);
	pCollider = new SphereCollider(this, position, 100);
}

void ItemShop::Update() {
	GameObject::Update();
	// オブジェクトに触れてボタンを押したら
	if (isTouch && state == Invalid && (InputManager::GetInstance().IsKeyDown(KEY_INPUT_F) || InputManager::GetInstance().IsButtonDown(XINPUT_GAMEPAD_B))) {
		state = Menu;
		OpenExecute();
		return;
	}

	// 戻るボタンを押したら
	if (isTouch && state != Invalid && (InputManager::GetInstance().IsKeyDown(KEY_INPUT_ESCAPE) || InputManager::GetInstance().IsButtonDown(XINPUT_GAMEPAD_A)))
		if (state != Menu)
			state = Menu;
		else {
			state = Invalid;
			GameSystem::GetInstance()->SetGameStatus(GameStatus::Playing); // ゲーム再開

		}

	switch (state) {
	case Menu:
		if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_UP) || InputManager::GetInstance().IsButtonDown(XINPUT_GAMEPAD_DPAD_UP) ||
			InputManager::GetInstance().IsKeyDown(KEY_INPUT_DOWN) || InputManager::GetInstance().IsButtonDown(XINPUT_GAMEPAD_DPAD_DOWN))
			if (selectCommand == 0) selectCommand = 1;
			else if (selectCommand == 1) selectCommand = 0;
			else selectCommand = 0;

		// 決定ボタンを押したら
		if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_F) || InputManager::GetInstance().IsButtonDown(XINPUT_GAMEPAD_B)) {
			switch (selectCommand) {
			case 0:
				state = Buy;
				break;
			case 1:
				state = Sell;
				break;
			}
		}

		break;
	case Buy:
		if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_LEFT) || InputManager::GetInstance().IsButtonDown(XINPUT_GAMEPAD_DPAD_LEFT)) {
			selectBuyCommand--;
			if ((selectBuyCommand == -1 || selectBuyCommand == 2))
				selectBuyCommand += 3;
		}
		if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_RIGHT) || InputManager::GetInstance().IsButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT)) {
			selectBuyCommand++;
			if ((selectBuyCommand == 3 || selectBuyCommand == 6))
				selectBuyCommand -= 3;
		}
		if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_UP) || InputManager::GetInstance().IsButtonDown(XINPUT_GAMEPAD_DPAD_UP)) {
			selectBuyCommand -= 3;
			if (selectBuyCommand <= -1)
				selectBuyCommand += 6;
		}
		if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_DOWN) || InputManager::GetInstance().IsButtonDown(XINPUT_GAMEPAD_DPAD_DOWN)) {
			selectBuyCommand += 3;
			if (selectBuyCommand > 5)
				selectBuyCommand -= 6;
		}

		// 決定
		if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_RETURN) || InputManager::GetInstance().IsButtonDown(XINPUT_GAMEPAD_B)) {
			Player* pP = Player::GetInstance();
			BuyItemData& buy = buyItem[selectBuyCommand];
			if (buy.prace <= pP->GetCoinValue() && !buy.isSell) {
				// コインを減らす

				// アイテムを渡す
				pP->GetInventory()->AddItem(std::move(buyItemData[selectBuyCommand]));
				buy.isSell = true;
			}
			else {
				// ビープ音を出す

			}
		}

		break;
	case Sell:
		break;
	}

	if (pCollider)
		pCollider->Update();
}

void ItemShop::Render() {

	if (!isTouch || state == Invalid) return;

	DrawFillBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, black);

	string titleStr;
	VECTOR strPos;
	switch (state) {
	case Menu:
		DrawStringToHandle(300, 500, "購入", white, MainFont);
		DrawStringToHandle(300, 600, "売却", white, MainFont);
		DrawString(280, 500 + (100 * selectCommand), ">", white);
		break;
	case Buy:
		titleStr = "購入";
		strPos = StringCenterPos(titleStr.c_str(), MainFont_Bold, WINDOW_WIDTH / 2, 100);
		DrawStringToHandle(strPos.x, strPos.y, titleStr.c_str(), white, MainFont_Bold);
		for (int i = 0, max = buyItem.size(); i < max; i++) {
			BuyItemData& item = buyItem[i];
			DrawBuyItem(item);
			// 購入済みじゃないなら
			if (!item.isSell) continue;
			string sold = "SOLD OUT!";
			int col = 3;
			int x = i % col;
			int y = i / col;
			VECTOR soldStrPos = VMult(buyItemIconOffSet, VGet(x + 1, y + 1, 0));
			soldStrPos = StringCenterPos(sold.c_str(), MainFont, soldStrPos.x, soldStrPos.y);
			DrawStringToHandle(soldStrPos.x, soldStrPos.y, sold.c_str(), red, MainFont);
		}

		VECTOR imgPos = buyItem[selectBuyCommand].imagePos;
		DrawLineBox(imgPos.x, imgPos.y, imgPos.x + 150, imgPos.y + 150, yellow);
		break;
	case Sell:
		break;
	}

#if _DEBUG
	if (pCollider)
		pCollider->Render();
#endif
}

void ItemShop::OnTriggerEnter(Collider* _pOhter) {
	if (!_pOhter->GetGameObject()->CompareTag("Player")) return;

	isTouch = true;
}

void ItemShop::OnTriggerExit(Collider* _pOhter) {
	if (!_pOhter->GetGameObject()->CompareTag("Player")) return;

	isTouch = false;
}
