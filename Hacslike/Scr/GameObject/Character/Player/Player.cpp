#include "Player.h"
#include "../../../Manager/FadeManager.h"
#include "../../Camera/Camera.h"
#include "../../../Component/Collider/Collider.h"
#include "../../../Definition.h"
#include "../Hacslike/Scr/Manager/CollisionManager.h"
#include "../Hacslike/Scr/Manager/TimeManager.h"
#include "../Hacslike/Scr/GameObject/Weapon/Weapon.h"
#include <math.h>


#define M_PI 3.14159265358979323846

// シングルトンインスタンスの初期化
Player* Player::instance = nullptr;

/*
 *	@brief		コンストラクタ
 *	@param[in]	VECTOR _pos		初期化する座標
 */
Player::Player(VECTOR _pos)
	: Character(_pos, "Player", Lv, Exp, speed)
	, pWeapon(nullptr)
	, input(&InputManager::GetInstance())
	//, slashes()								//	斬撃
	, currentWeaponId()
	, changeWeaponButtonPressed(false)
	, hitItem(false)
	, isItemUI(false)
	, coinValue()
	, expValue()
	, criticalHitRate()
	, criticalDamage()
	, weaponData(nullptr)
	, hpRate() {
	maxHp = 100;
	hp = maxHp;
	atk = 5;
	def = 2;
	Exp = 0;
	criticalHitRate = 10;
	criticalDamage = 100;
	// コンストラクタでシングルトンの重複生成を防ぐ
	if (instance != nullptr) {
		printfDx("[Error] Player instance already exists!\n");
	}
	else {
		instance = this;
	}
	Start();

}

/*
 *	@breif		デストラクタ
 */
Player::~Player() {
	delete pWeapon;
	pWeapon = nullptr;

	if (instance == this) {
		instance = nullptr;
	}

	MV1DeleteModel(PLAYER_MODEL_HANDLE);
}

Player* Player::CreateInstance(VECTOR _pos) {
	if (!instance) {
		instance = new Player(_pos);
	}
	return instance;
}

Player* Player::GetInstance() {
	return instance;
}

void Player::DestroyInstance() {
	delete instance;
	instance = nullptr;
}

/*
 *	@function	Start
 *	@breif		初期化処理
 */
void Player::Start() {
	//	非表示だったら初期化しない
	if (!isVisible)
		return;
	SetCollider(new CapsuleCollider(this, VZero, VScale(VUp, 200), 50.0f));
	CollisionManager::GetInstance().Register(pCollider);

	modelHandle = PLAYER_MODEL_HANDLE;

	pAnimator->SetModelHandle(modelHandle);

	SetPlayer(this);


	//	アニメーションの読み込み
	GetAnimator()->Load("Res/PlayerModel/Neutral.mv1", "Idle", true);
	GetAnimator()->Load("Res/PlayerModel/Walking.mv1", "Walk", true);
	GetAnimator()->Load("Res/PlayerModel/Attack1.mv1", "Atk1");
	GetAnimator()->Load("Res/PlayerModel/Attack2.mv1", "Atk2");
	GetAnimator()->Load("Res/PlayerModel/Attack3.mv1", "Atk3");
	GetAnimator()->Load("Res/PlayerModel/Run.mv1", "Run", true);
	GetAnimator()->Load("Res/PlayerModel/AxeAttack1.mv1", "AxeAtk1");
	GetAnimator()->Load("Res/PlayerModel/AxeAttack3.mv1", "AxeAtk2");
	GetAnimator()->Load("Res/PlayerModel/AxeAttack2.mv1", "AxeAtk3");
	GetAnimator()->Load("Res/PlayerModel/AxeAttack4.mv1", "AxeAtk4");
	GetAnimator()->Load("Res/PlayerModel/GreatAttack1.mv1", "GreatAtk1");
	GetAnimator()->Load("Res/PlayerModel/GreatAttack2.mv1", "GreatAtk2");
	GetAnimator()->Load("Res/PlayerModel/GreatAttack3.mv1", "GreatAtk3");
	GetAnimator()->Load("Res/PlayerModel/GreatAttack4.mv1", "GreatAtk4");
	GetAnimator()->Load("Res/PlayerModel/GreatCharge1.mv1", "GreatCharge1");
	GetAnimator()->Load("Res/PlayerModel/GreatCharge2.mv1", "GreatCharge2", true);
	GetAnimator()->Load("Res/PlayerModel/GreatCharge3.mv1", "GreatCharge3");

	pAnimator->Play(0);

	WeaponManager::GetInstance().LoadWeapons("Scr/Data/WeaponsData.json");
	maxWeaponId = WeaponManager::GetInstance().GetMaxWeaponId();
	changeWeaponButtonPressed = false;
	currentWeaponId = 10;
	weaponData = WeaponManager::GetInstance().GetWeapon(currentWeaponId);
	if (weaponData) {
		pWeapon = new Weapon(weaponData->name, weaponData->modelHandle);
		pWeapon->SetColLength(weaponData->colLength);
		pWeapon->SetColRadius(weaponData->colRadius);
		pWeapon->SetType((WeaponType)weaponData->type);
		pWeapon->SetAnimationSpeed(weaponData->attackSpeed);
		pWeapon->attach(modelHandle, pWeapon->GetModelHandle(), "wp", this);
	}

	playerMovement = new PlayerMovement(this);
	playerAttack = new PlayerAttack(this, pWeapon, playerMovement);

	SetSpeed(1);
}

/*
 *	@function	Update
 *	@breif		更新処理
 */
void Player::Update() {
	//	非表示だったら更新しない
	if (!isVisible)
		return;

	hpRate = (float)hp / (float)maxHp;

	if (hp > maxHp) {
		hp = maxHp;
	}


	////	攻撃入力・HitBox更新
	//AttackInput();


	playerMovement->Update();
	playerAttack->Update();

	//CheckWall();

	//	斬撃更新
	//UpdateSlash();

	MV1SetMatrix(modelHandle, matrix);

	WeaponInput();

	//アイテムの取得
	AddItem();
	//アイテムのインベントリ表示非表示
	//OpenInventory();

	selectMenu();

	OpenMenu();

	if(isItemUI) {
		inventory.Update(this);
	}

	if (isArtifactUI) {
		artifactUI.Update();
	}

	if (input->IsButtonDown(XINPUT_GAMEPAD_Y) || input->IsKeyDown(KEY_INPUT_1)) {
		AddHp(10);
	}
	else if (input->IsKeyDown(KEY_INPUT_2)) {
		SubHp(10);
	}

	pAnimator->Update();

	GameObject::Update();

	//	武器の更新
	if (pCollider != nullptr && pWeapon != nullptr) {
		pWeapon->Update();
	}

	if (pCollider != nullptr) {
		pCollider->Update();
	}

}


/*
 *	@function	Render
 *	@breif		描画処理
 */
void Player::Render() {
	//	非表示だったら描画しない
	if (!isVisible)
		return;
#pragma region アイテムのインベントリ表示
	if (isItemUI) {
		inventory.Render();
	}
	if (isArtifactUI) {
		artifactUI.Render();
	}
	inventory.AddItemRender();

	if (isMenuUI) {
		DrawMenu();
	}
	int cx = 0, cy = 800;   // 中心
	int r_outer = 200;
	int r_inner = 120;
	int steps = 50;

	double percent = (double)hp / maxHp * 100;
	double max_angle = 90.0;
	double angle_end = max_angle * percent / 100.0;

	for (int i = 0; i < steps; i++) {
		double angle1 = (angle_end * i / steps) * M_PI / 180.0;
		double angle2 = (angle_end * (i + 1) / steps) * M_PI / 180.0;

		// 外側頂点
		int x1o = cx + (int)(r_outer * cos(angle1));
		int y1o = cy - (int)(r_outer * sin(angle1)); // sinの符号を反転
		int x2o = cx + (int)(r_outer * cos(angle2));
		int y2o = cy - (int)(r_outer * sin(angle2));

		// 内側頂点
		int x1i = cx + (int)(r_inner * cos(angle1));
		int y1i = cy - (int)(r_inner * sin(angle1));
		int x2i = cx + (int)(r_inner * cos(angle2));
		int y2i = cy - (int)(r_inner * sin(angle2));

		// 三角形2つで四角形を描く
		DrawTriangle(x1o, y1o, x2o, y2o, x1i, y1i, green, TRUE);
		DrawTriangle(x2o, y2o, x2i, y2i, x1i, y1i, green, TRUE);
	}
#pragma endregion

	playerMovement->Render();

#pragma region プレイヤーの描画
	// --- 通常モデル描画 ---
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	MV1SetMatrix(modelHandle, matrix);
	MV1DrawModel(modelHandle);
#pragma endregion

	//#pragma region Slash描画
	//	for (auto s : slashes) {
	//		s->Render();
	//	}
	//#pragma endregion
	//
	//#pragma endregion

#pragma region 武器の描画
	//	武器の描画
	if (pWeapon != nullptr) {
		pWeapon->Render();
	}
#pragma endregion


}

///// <summary>
///// 斬撃更新
///// </summary>
//void Player::UpdateSlash() {
//	for (auto it = slashes.begin(); it != slashes.end();) {
//		Slash* s = *it;
//		s->Update();
//
//		if (s->IsDead()) {
//			delete s;
//			it = slashes.erase(it);
//		}
//		else {
//			++it;
//		}
//	}
//}

/// <summary>
/// 武器切り替え
/// </summary>
/// <param name="weaponId"></param>
void Player::ChangeWeapon(int weaponId) {
	weaponData = WeaponManager::GetInstance().GetWeapon(weaponId);
	if (!weaponData) return;

	pWeapon->ChangeModel(weaponData->modelHandle);

	pWeapon->SetType(static_cast<WeaponType>(weaponData->type));
	pWeapon->SetColLength(weaponData->colLength);
	pWeapon->SetColRadius(weaponData->colRadius);
	pWeapon->SetAnimationSpeed(weaponData->attackSpeed);

	playerAttack->SetWeapon(pWeapon);
}

/// <summary>
/// 武器切り替え入力
/// </summary>
void Player::WeaponInput() {
	if ((input->IsKeyDown(KEY_INPUT_C) && !changeWeaponButtonPressed ||
		input->IsButtonDown(XINPUT_GAMEPAD_BACK) && !changeWeaponButtonPressed) && !playerAttack->IsAttacking()) {
		changeWeaponButtonPressed = true;

		currentWeaponId++;
		if (currentWeaponId > maxWeaponId)
			currentWeaponId = 1;

		ChangeWeapon(currentWeaponId);
	}
	else if (!input->IsKeyDown(KEY_INPUT_C)) {
		changeWeaponButtonPressed = false;
	}
}

/// <summary>
/// アイテムの取得
/// </summary>
void Player::AddItem() {
	auto& items = ItemDropManager::GetInstance()->GetActiveItems();

	for (auto& item : items) {
		if (hitItem && (input->IsKeyDown(KEY_INPUT_F) || input->IsButtonDown(XINPUT_GAMEPAD_B))) {
			item->SetVisible(false);
			std::string itemName = item->GetItem()->GetName();

			// インベントリへ追加
			GetInventory()->AddItem(std::move(item->TakeItem()));


			ItemDropManager::GetInstance()->RemoveItem(item.get());
			break; // erase後にvectorを操作しないようにbreak
		}
	}
}

void Player::OpenMenu()
{
	// TAB または START でメニュー開閉
	if (((input->IsKeyDown(KEY_INPUT_TAB)) || input->IsButtonDown(XINPUT_GAMEPAD_START)) && !isMenuUI) {
		isMenuUI = true;
	}
	else if (((input->IsKeyDown(KEY_INPUT_TAB)) || input->IsButtonDown(XINPUT_GAMEPAD_START)) && isMenuUI) {
		isMenuUI = false;
		isItemUI = false;
		isArtifactUI = false;
		isMenuSelected = false; // 閉じたときにリセット
	}

	if (isMenuUI)
	{
		// メニュー操作
		if (!isMenuSelected) // 選択中はカーソル操作を無効に
		{
			// ↑キー
			if (input->IsKeyDown(KEY_INPUT_UP) || input->IsButtonDown(XINPUT_GAMEPAD_DPAD_UP))
			{
				if (menu == MenuType::menuArtifact)
					menu = MenuType::menuInventory;
			}

			// ↓キー
			if (input->IsKeyDown(KEY_INPUT_DOWN) || input->IsButtonDown(XINPUT_GAMEPAD_DPAD_DOWN))
			{
				if (menu == MenuType::menuInventory)
					menu = MenuType::menuArtifact;
			}

			// Enterで選択
			if (input->IsKeyUp(KEY_INPUT_RETURN) || input->IsButtonUp(XINPUT_GAMEPAD_B))
			{
				isMenuSelected = true;
				blinkTime = 0.0f;
				blinkVisible = true;
			}
		}

		// Backspaceで解除
		if (input->IsKeyDown(KEY_INPUT_BACK) || input->IsButtonUp(XINPUT_GAMEPAD_A))
		{
			isMenuSelected = false;
			blinkVisible = true; // 常に表示状態に戻す
		}
	}

	// 点滅制御
	if (isMenuSelected)
	{
		blinkTime += 1.0f / 60.0f; // 1フレーム1/60秒進む想定
		if (blinkTime >= 0.3f) // 約0.3秒ごとに切り替え
		{
			blinkVisible = !blinkVisible;
			blinkTime = 0.0f;
		}
	}
	else
	{
		blinkVisible = true;
	}
}

void Player::selectMenu()
{
	if (isMenuUI == true) {
		switch (menu)
		{
		case menuInventory:
			isItemUI = true;
			isArtifactUI = false;
			break;
		case menuArtifact:
			isItemUI = false;
			isArtifactUI = true;
			break;
		default:
			break;
		}
	}
	else return;
}

void Player::DrawMenu()
{
	if (!isMenuUI) return;

	const int x = 20;
	const int y = 50;
	const int width = 200;
	const int height = 40;
	const int margin = 10;

	const char* menuNames[] = { "アイテム", "アーティファクト" };
	const int menuCount = 2;

	for (int i = 0; i < menuCount; i++)
	{
		int boxY = y + i * (height + margin);

		bool isCurrent = (static_cast<int>(menu) == i);

		// 選択中かつ点滅表示がオンの時のみピンク枠
		if (isCurrent && (!isMenuSelected || (isMenuSelected && blinkVisible)))
		{
			DrawBox(x - 4, boxY - 4, x + width + 4, boxY + height + 4, GetColor(255, 0, 255), TRUE);
			DrawBox(x, boxY, x + width, boxY + height, GetColor(0, 0, 0), TRUE);
		}
		else
		{
			DrawBox(x - 2, boxY - 2, x + width + 2, boxY + height + 2, GetColor(255, 255, 255), FALSE);
			DrawBox(x, boxY, x + width, boxY + height, GetColor(0, 0, 0), TRUE);
		}

		DrawString(x + 40, boxY + 10, menuNames[i], GetColor(255, 255, 255));
	}
}

void Player::GetCoin()
{
	if (coinArtifact)
		coinArtifact->OnGetCoin(this);
}

void Player::GetCoin_Item()
{
	if (itemArtifact)
		itemArtifact->OnGetCoin_Item(this);
}



void Player::PlayerStatusRender() {


	DrawFormatString(960, 20, green, "LV　　　　　 : %d", Lv);
	DrawFormatString(960, 40, green, "EXP　　　　　: %d", Exp);
	DrawFormatString(960, 60, green, "HP　　　　　 : %d / %d", hp, maxHp);
	DrawFormatString(960, 80, green, "攻撃力　　　 : %d", atk);
	DrawFormatString(960, 100, green, "防御力　　　 : %d", def);
	DrawFormatString(960, 120, green, "会心率　　　 : %.1f", criticalHitRate);
	DrawFormatString(960, 140, green, "会心ダメージ : %.1f", criticalDamage);
	DrawFormatString(960, 160, green, "コイン　　　 : %d", coinValue);

	
}

/*
 *	@function	OnTriggerEnter
 *	@brief		当たった瞬間
 *	@param[in]	Collider* _pCol
 */
void Player::OnTriggerEnter(Collider* _pCol) {
	//	当たった相手のタグが "Goblin" だったら
	if (_pCol->GetGameObject()->GetTag() == "Goblin") {

	}
	if (_pCol->GetGameObject()->GetTag() == "Coin") {


		auto& coins = Coin::GetInstance()->GetCoin();

		for (auto& coin : coins) {
			// 衝突したコインオブジェクトと一致した場合のみ処理
			if (coin.get() == _pCol->GetGameObject()) {
				coin->SetVisible(false);
				coin->ApplyCoin(this);
				GetCoin();  // ← プレイヤー経由で発動
				GetCoin_Item();
				Coin::GetInstance()->RemoveCoin(coin.get());
				break; // 削除したのでループを抜ける
			}
		}
	}
}

/*wsa
 *	@function	OnTriggerSaty
 *	@brief		当たっている間
 *	@param[in]	Collider* _pCol
 */
void Player::OnTriggerStay(Collider* _pCol) {
	if (_pCol->GetGameObject()->GetTag() == "item") {
		hitItem = true;
	}
}

/*
 *	@function	OnTriggerExit
 *	@brief		離れた瞬間
 *	@param[in]	Collider* _pCol
 */
void Player::OnTriggerExit(Collider* _pCol) {
	if (_pCol->GetGameObject()->GetTag() == "item") {
		hitItem = false;

	}
}
