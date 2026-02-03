#include "ItemShop.h"
#include "../../CommonModule.h"
#include "../Item/ItemFactory.h"
#include "../../Manager/InputManager.h"
#include "../Character/Player/Player.h"
#include "../Item/Inventory.h"
#include "../../Component/Collider/Collider.h"
#include "../../GameSystem/GameSystem.h"
#include "../../Manager/SalesManager.h"

ItemShop::ItemShop()
	:isTouch(false)
	, isChoice(false)
	, state(Invalid) {
	Start();
}

ItemShop::~ItemShop() {}

void ItemShop::ChoiceItem(std::vector<std::string> _itemIDList, int _choiceCount) {
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

void ItemShop::DeadExecute() {}

void ItemShop::OpenExecute() {
	GameSystem::GetInstance()->SetGameStatus(GameStatus::Stop); // ゲーム停止

	Setup();

	if (!isChoice) {
		isChoice = true;

		ChoiceItem(potionIDList, 3);
		ChoiceItem(weaponIDList, 2);
		ChoiceItem(allItemIDList, 1);
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
	int mHandle = MV1LoadModel("Res/Model/Shop/vending.mv1");
	SetModelHandle(mHandle);
	SetScale(VGet(150, 150, 150));
	SetRotation(VGet(0, 90, 0));
}

void ItemShop::Update() {
	GameObject::Update();

	if (pCollider)
		pCollider->Update();

	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);
	auto& input = InputManager::GetInstance();

	// オブジェクトに触れてボタンを押したら
	if (isTouch && state == Invalid && (InputManager::GetInstance().IsKeyDown(KEY_INPUT_F) || InputManager::GetInstance().IsButtonDown(XINPUT_GAMEPAD_B) || InputManager::GetInstance().IsKeyDown(KEY_INPUT_RETURN))) {
		state = Menu;
		OpenExecute();
		SetMouseDispFlag(TRUE);
		return;
	}

	// 戻るボタンを押したら
	if (isTouch && state != Invalid && (InputManager::GetInstance().IsKeyDown(KEY_INPUT_ESCAPE) || InputManager::GetInstance().IsButtonDown(XINPUT_GAMEPAD_A)))
		if (state != Menu)
			state = Menu;
		else {
			state = Invalid;
			GameSystem::GetInstance()->SetGameStatus(GameStatus::Playing); // ゲーム再開
			SetMouseDispFlag(FALSE);
			return;
		}

	if (state == Invalid) return;

	if (GameSystem::GetInstance()->GetGameStatus() == GameStatus::Playing)
		GameSystem::GetInstance()->SetGameStatus(GameStatus::Stop);

	switch (state) {
	case Menu:
		if (mouseY >= 480 && mouseY <= 550) { // 「購入」の高さ
			selectCommand = 0;
			if (input.IsMouseDown(MOUSE_INPUT_LEFT)) state = Buy;
		}
		else if (mouseY >= 630 && mouseY <= 700) { // 「売却」の高さ
			selectCommand = 1;
			if (input.IsMouseDown(MOUSE_INPUT_LEFT)) state = Sell;
		}

		if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_UP) || InputManager::GetInstance().IsButtonDown(XINPUT_GAMEPAD_DPAD_UP) ||
			InputManager::GetInstance().IsKeyDown(KEY_INPUT_DOWN) || InputManager::GetInstance().IsButtonDown(XINPUT_GAMEPAD_DPAD_DOWN))
			if (selectCommand == 0) selectCommand = 1;
			else if (selectCommand == 1) selectCommand = 0;
			else selectCommand = 0;

		// 決定ボタンを押したら
		if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_F) || InputManager::GetInstance().IsButtonDown(XINPUT_GAMEPAD_B) || InputManager::GetInstance().IsKeyDown(KEY_INPUT_RETURN)) {
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
		for (int i = 0; i < (int)buyItem.size(); i++) {
			BuyItemData& item = buyItem[i];
			// アイテム画像の描画範囲 (150x150) にマウスがあるか判定
			if (mouseX >= item.imagePos.x && mouseX <= item.imagePos.x + 150 &&
				mouseY >= item.imagePos.y && mouseY <= item.imagePos.y + 150) {

				selectBuyCommand = i; // マウスが乗っているアイテムを選択状態にする

				// 左クリックで購入
				if (input.IsMouseDown(MOUSE_INPUT_LEFT) && !item.isSell) {
					Player* pP = Player::GetInstance();
					if (item.prace <= pP->GetCoinValue()) {
						pP->SubCoinValue(item.prace);
						pP->GetInventory()->AddItem(std::move(buyItemData[i]));
						item.isSell = true;
					}
				}
			}
		}

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
				pP->SubCoinValue(buy.prace);
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
		if (!SalesManager::GetInstance().IsActive()) {
			SalesManager::GetInstance().StartSellScene(Player::GetInstance()->GetInventory());
		}
		SalesManager::GetInstance().Update();
		break;
	}
}

void ItemShop::Render() {

	MV1SetMatrix(modelHandle, matrix);
	MV1DrawModel(modelHandle);

	if (!isTouch && state == Invalid) return;

	if (isTouch && state == Invalid) {
		VECTOR screenPos = ConvWorldPosToScreenPos(VAdd(position, VGet(0, 100, 0)));
		int StartX = (WINDOW_WIDTH / 2) - 200;
		int StartY = (WINDOW_HEIGHT)-200;
		int GoalX = (WINDOW_WIDTH / 2) + 200;
		int GoalY = (WINDOW_HEIGHT)-150;
		int textX = StartX + 80;
		int textY = StartY + 17;

		DrawBox(StartX, StartY, GoalX, GoalY, gray, TRUE);
		DrawBox(StartX + 2, StartY + 2, GoalX - 2, GoalY - 2, white, FALSE);
		DrawFormatStringToHandle(textX + 40, textY, black, MainFont, "キー/  ボタン:ショップ");
		DrawFormatStringToHandle(textX + 30, textY, white, MainFont, "F");
		DrawFormatStringToHandle(textX + 83, textY, white, MainFont, "B");
		return;
	}

	SetUseZBuffer3D(FALSE);
	SetWriteZBuffer3D(FALSE);
	ClearDrawScreenZBuffer();
	SetUseLighting(FALSE);

	DrawFillBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, black);

	string titleStr;
	VECTOR strPos;
	string buyTitle = "購入";
	string sellTitle = "売却";
	switch (state) {
	case Menu:
		titleStr = "ショップ";
		strPos = StringCenterPos(titleStr.c_str(), MainFont_Bold, WINDOW_WIDTH / 2, 100);
		DrawStringToHandle(strPos.x, strPos.y, titleStr.c_str(), white, MainFont_Bold);
		VECTOR buyTitlePos = StringCenterPos(buyTitle.c_str(), MainFont_Bold, WINDOW_WIDTH / 2, 500);
		VECTOR sellTitlePos = StringCenterPos(sellTitle.c_str(), MainFont_Bold, WINDOW_WIDTH / 2, 650);
		if (selectCommand == 0) {
			DrawStringToHandle(buyTitlePos.x, buyTitlePos.y, buyTitle.c_str(), yellow, MainFont_Bold);
			DrawStringToHandle(sellTitlePos.x, sellTitlePos.y, sellTitle.c_str(), white, MainFont_Bold);
		}
		else if (selectCommand == 1) {
			DrawStringToHandle(buyTitlePos.x, buyTitlePos.y, buyTitle.c_str(), white, MainFont_Bold);
			DrawStringToHandle(sellTitlePos.x, sellTitlePos.y, sellTitle.c_str(), yellow, MainFont_Bold);
		}
		break;
	case Buy:
		// 所持コイン表示 (右上)

		DrawFormatStringToHandle(WINDOW_WIDTH - 280, 60, coinColor, MainFont, "COINS: %d", Player::GetInstance()->GetCoinValue());

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
		SalesManager::GetInstance().Render();
		break;
	}

#if _DEBUG
	if (pCollider)
		pCollider->Render();
#endif

	SetUseLighting(TRUE);
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
}

void ItemShop::OnTriggerEnter(Collider* _pOhter) {
	if (!_pOhter->GetGameObject()->CompareTag("Player")) return;

	isTouch = true;
}

void ItemShop::OnTriggerExit(Collider* _pOhter) {
	if (!_pOhter->GetGameObject()->CompareTag("Player")) return;

	isTouch = false;
}
