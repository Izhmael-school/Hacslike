#include "Player.h"
#include "../../../Manager/FadeManager.h"
#include"../../../Manager/SkillManager.h"
#include "../../Camera/Camera.h"
#include "../../../Component/Collider/Collider.h"
#include "../../../Definition.h"
#include "../Hacslike/Scr/Manager/CollisionManager.h"
#include "../Hacslike/Scr/Manager/TimeManager.h"
#include "../Hacslike/Scr/GameObject/Weapon/Weapon.h"
#include"../../Item/ItemFactory.h"
#include <math.h>
#include "../../Item/ItemEquip/ItemEquip.h"

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
	, isItemUI(false)
	, coinValue()
	, expValue()
	, criticalHitRate()
	, criticalDamage()
	, weaponData(nullptr)
	, hpRate()
	, maxExp()
	, remainExp(){
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

void Player::IsDead() {
	if (hp > 0) return;
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
	//CollisionManager::GetInstance().Register(pCollider);

	modelHandle = PLAYER_MODEL_HANDLE;

	pAnimator->SetModelHandle(modelHandle);

	SetPlayer(this);

	maxExp = 100;


	//	アニメーションの読み込み
	GetAnimator()->Load("Res/PlayerModel/Neutral.mv1", "Idle",true, true);
	GetAnimator()->Load("Res/PlayerModel/Walking.mv1", "Walk",true, true);
	GetAnimator()->Load("Res/PlayerModel/Attack1.mv1", "Atk1",true);
	GetAnimator()->Load("Res/PlayerModel/Attack2.mv1", "Atk2",true);
	GetAnimator()->Load("Res/PlayerModel/Attack3.mv1", "Atk3",true);
	GetAnimator()->Load("Res/PlayerModel/Run.mv1", "Run", true);
	GetAnimator()->Load("Res/PlayerModel/AxeAttack1.mv1", "AxeAtk1",true);
	GetAnimator()->Load("Res/PlayerModel/AxeAttack3.mv1", "AxeAtk2",true);
	GetAnimator()->Load("Res/PlayerModel/AxeAttack2.mv1", "AxeAtk3",true);
	GetAnimator()->Load("Res/PlayerModel/AxeAttack4.mv1", "AxeAtk4",true);
	GetAnimator()->Load("Res/PlayerModel/GreatAttack1.mv1", "GreatAtk1",true);
	GetAnimator()->Load("Res/PlayerModel/GreatAttack2.mv1", "GreatAtk2",true);
	GetAnimator()->Load("Res/PlayerModel/GreatAttack3.mv1", "GreatAtk3",true);
	GetAnimator()->Load("Res/PlayerModel/GreatAttack4.mv1", "GreatAtk4",true);
	GetAnimator()->Load("Res/PlayerModel/GreatCharge1.mv1", "GreatCharge1",true);
	GetAnimator()->Load("Res/PlayerModel/GreatCharge2.mv1", "GreatCharge2",true, true);
	GetAnimator()->Load("Res/PlayerModel/GreatCharge3.mv1", "GreatCharge3",true);

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
	// 木の棒アイテムを生成（ItemFactory を使う場合は CreateItem でも可）
	std::unique_ptr<ItemBase> stick = std::make_unique<ItemStick>(
		VZero, "木の棒", "そこら辺に落ちてる木の棒", 0, 5, "Res/ItemIcon/stick.png"
	);

	// インベントリに追加
	GetInventory()->AddItem(std::move(stick));

	// 追加されたアイテムは items.back() に入っているので取得
	
	Inventory::InventoryItem* lastItem = GetInventory()->GetLastItem();

		// 装備（EquipItem は Use() を内部で呼ぶ）
		GetInventory()->EquipItem(lastItem->item.get());
	

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

	ArtifactManager::GetInstance()->Update(this);
	
	

	selectMenu();

	OpenMenu();

	inventory.Update(this);
	if(isItemUI) {
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

	if (input->IsKeyDown(KEY_INPUT_3)) {
		AddExp(15);
	}

#pragma region スキル選択
	if (Exp >= maxExp && !isSelectingSkill) {
		remainExp = Exp - maxExp;
		Exp = remainExp;
		//maxExp *= 1.1;
		skillChoices = SkillManager::GetInstance().GenerateSkillChoices();
		skillUI.StartSelection();
		isSelectingSkill = true;
	}
	if (isSelectingSkill) {
		// ★スキル選択中の処理
		int selected = skillUI.UpdateSelection();
		if (selected != -1)
		{

			if (player && selected >= 0 && selected < (int)skillChoices.size())
			{
				SkillManager::GetInstance().ApplySelectedSkill(this, skillChoices[selected]);
			}
			isSelectingSkill = false;

		}
	}
#pragma endregion
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

#pragma region プレイヤーのHPやEXP
	int cx = 0, cy = 800;   //	中心座標
	int r_outer = 200;		//	外側半径
	int r_inner = 160;		//	内側半径
	int steps = 50;			//	分割数

	double percent = (double)hp / maxHp * 100;		//	HP割合
	double max_angle = 90.0;						//	四分円の角度
	double angle_end = max_angle * percent / 100.0;	//	HP割合に応じて何度まで描画するか

	for (int i = 0; i < steps; i++) {
		double angle1 = Deg2Rad(angle_end * i / steps);			//	ラジアン角への変換
		double angle2 = Deg2Rad(angle_end * (i + 1) / steps);	//	ラジアン角への変換

		// 外側頂点
		int x1o = cx + (int)(r_outer * cos(angle1));
		int y1o = cy - (int)(r_outer * sin(angle1)); 
		int x2o = cx + (int)(r_outer * cos(angle2));
		int y2o = cy - (int)(r_outer * sin(angle2));

		// 内側頂点
		int x1i = cx + (int)(r_inner * cos(angle1));
		int y1i = cy - (int)(r_inner * sin(angle1));
		int x2i = cx + (int)(r_inner * cos(angle2));
		int y2i = cy - (int)(r_inner * sin(angle2));

		// 三角形2つで四角形を描画
		DrawTriangle(x1o, y1o, x2o, y2o, x1i, y1i, GetColor(30, 200, 30), TRUE);
		DrawTriangle(x2o, y2o, x2i, y2i, x1i, y1i, GetColor(30, 200, 30), TRUE);
	}

	int expR_outer = 160;  //	外側半径
	int expR_inner = 140;  //	内側半径
	int expSteps = 50;	   //	分割数

	double expPercent = (double)Exp / maxExp * 100;
	double Exp_max_angle = 90.0;
	double Exp_angle_end = Exp_max_angle * expPercent / 100.0;

	for (int i = 0; i < expSteps; i++) {
		double expAngle1 = Deg2Rad(Exp_angle_end * i / expSteps);		//	ラジアン角への変換
		double expAngle2 = Deg2Rad(Exp_angle_end * (i + 1) / expSteps);	//	ラジアン角への変換

		//	外側頂点
		int expX1o = cx + (int)(expR_outer * cos(expAngle1));
		int expY1o = cy - (int)(expR_outer * sin(expAngle1));
		int expX2o = cx + (int)(expR_outer * cos(expAngle2));
		int expY2o = cy - (int)(expR_outer * sin(expAngle2));

		//	内側頂点
		int expX1i = cx + (int)(expR_inner * cos(expAngle1));
		int expY1i = cy - (int)(expR_inner * sin(expAngle1));
		int expX2i = cx + (int)(expR_inner * cos(expAngle2));
		int expY2i = cy - (int)(expR_inner * sin(expAngle2));

		//	三角形2つで四角形を描画
		DrawTriangle(expX1o, expY1o, expX2o, expY2o, expX1i, expY1i, yellow, TRUE);
		DrawTriangle(expX2o, expY2o, expX2i, expY2i, expX1i, expY1i, yellow, TRUE);
	}
		DrawCircle(cx, cy, r_outer, white, FALSE);
		DrawCircle(cx, cy, r_inner, white, FALSE);
		DrawCircle(cx, cy, expR_inner, white, FALSE);

#pragma endregion		


	playerMovement->Render();

	DrawFormatString(200, 200, red, "x : %d | z : %d", (int)position.x, (int)position.z);
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

#pragma region アイテムのインベントリ表示
	if (isMenuUI) {
		DrawMenu();
	}
	if (isItemUI) {
		inventory.Render();
	}
	if (isArtifactUI) {
		artifactUI.Render();
	}

	inventory.AddItemRender();

	if (isSelectingSkill)
	{
		skillUI.Render(skillChoices);
	}

	AddItemRender();
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
	if (!hitItem) return;

	if (input->IsKeyDown(KEY_INPUT_F) || input->IsButtonDown(XINPUT_GAMEPAD_B)) {

		hitItem->SetVisible(false);

		// インベントリへ追加
		GetInventory()->AddItem(std::move(hitItem->TakeItem()));

		// DropManager から削除
		ItemDropManager::GetInstance().RemoveItem(hitItem);

		hitItem = nullptr; // 念のため解除
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
	PlayerStatusRender();
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

void Player::AddItemRender()
{
	int StartX = (WINDOW_WIDTH / 2) - 200;
	int StartY = (WINDOW_HEIGHT)-200;
	int GoalX = (WINDOW_WIDTH / 2) +200;
	int GoalY = (WINDOW_HEIGHT)-150;
	int textX = StartX + 80;
	int textY = StartY + 17;
	if (hitItem) {
		DrawBox(StartX,StartY,GoalX,GoalY, gray,TRUE);
		DrawBox(StartX+2, StartY+2, GoalX-2, GoalY-2, white, FALSE);
		DrawFormatString(textX+10, textY,black,"キー/ ボタン:アイテムを取る");
		DrawFormatString(textX, textY, white, "F");
		DrawFormatString(textX+53, textY, white , "Y");
		

	}
	else return;
}


void Player::PlayerStatusRender() {

	DrawBox(920, 20, WINDOW_WIDTH, 280, black, TRUE);
	DrawBox(920, 20, WINDOW_WIDTH, 40, white, FALSE);
	DrawFormatString(930, 20, white, "ステータス");
	DrawFormatString(930, 60, green, "LV　　　　　 : %d", Lv);
	DrawFormatString(930, 80, green, "EXP　　　　　: %d", Exp);
	DrawFormatString(930, 100, green, "HP　　　　　 : %d / %d", hp, maxHp);
	DrawFormatString(930, 120, green, "攻撃力　　　 : %d", atk);
	DrawFormatString(930, 140, green, "防御力　　　 : %d", def);
	DrawFormatString(930, 160, green, "会心率　　　 : %.1f", criticalHitRate);
	DrawFormatString(930, 180, green, "会心ダメージ : %.1f", criticalDamage);
	DrawFormatString(930, 200, green, "コイン　　　 : %d", coinValue);
	DrawFormatString(930, 220, green, "近距離補正　 : %f", proximityCorrection);
	DrawFormatString(930, 240, green, "遠距離補正　 : %f", rangedCorrection);



	
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
				coin->ApplyCoin(this);
				GetCoin();  // ← プレイヤー経由で発動
				GetCoin_Item();
				coin.get()->SetVisible(false);
				coin.get()->SetActive(false);
				coin.get()->GetCollider()->SetEnable(false);
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
		// 触れているアイテムを記録
		hitItem = dynamic_cast<ItemEntity*>(_pCol->GetGameObject());
	}
}

/*
 *	@function	OnTriggerExit
 *	@brief		離れた瞬間
 *	@param[in]	Collider* _pCol
 */
void Player::OnTriggerExit(Collider* _pCol) {
	if (_pCol->GetGameObject()->GetTag() == "item") {
		// 離れたアイテムなら解除
		if (hitItem == _pCol->GetGameObject()) {
			hitItem = nullptr;
		}
	}
}
