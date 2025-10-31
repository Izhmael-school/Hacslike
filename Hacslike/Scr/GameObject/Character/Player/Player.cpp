#include "Player.h"
#include "../../../Manager/FadeManager.h"
#include "../../Camera/Camera.h"
#include "../../../Component/Collider/Collider.h"
#include "../../../Definition.h"
#include "../Hacslike/Scr/Manager/CollisionManager.h"
#include "../Hacslike/Scr/Manager/WeaponManager.h"
#include "../Hacslike/Scr/Manager/TimeManager.h"
#include "../Hacslike/Scr/GameObject/Weapon/Weapon.h"

/*
 *	@brief		コンストラクタ
 *	@param[in]	VECTOR _pos		初期化する座標
 */
Player::Player(VECTOR _pos)
	: Character(_pos, "Player", Lv, Exp, speed)
	, pWeapon(nullptr)
	, XY()
	, inputVec()
	, input(InputManager::GetInstance())
	//, slashes()								//	斬撃
	, isBlinking(false)						//	回避の長押し防止
	, blinkTimer(0.0f)						//	回避のクールダウン
	, evasionButtonPressed(false)			//	
	, evasionCooldown(0.0f)
	, evasionSpeed(1.0f)
	, currentWeaponId()
	, changeWeaponButtonPressed(false)
	, hitItem(false)
	, isItemUI(false)
	, coinValue()
	, expValue()
	, criticalHitRate()
	, criticalDamage() {
	maxHp = 100;
	hp = maxHp;
	atk = 5;
	def = 2;
	Exp = 0;
	criticalHitRate = 10;
	criticalDamage = 100;
	Start();

}

/*
 *	@breif		デストラクタ
 */
Player::~Player() {
	delete pWeapon;
	pWeapon = nullptr;

	MV1DeleteModel(PLAYER_MODEL_HANDLE);
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
	CollisionManager::GetInstance()->Register(pCollider);

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
	GetAnimator()->Load("Res/PlayerModel/GreatAttack1.mv1", "GreatAtk1");
	GetAnimator()->Load("Res/PlayerModel/GreatAttack2.mv1", "GreatAtk2");
	GetAnimator()->Load("Res/PlayerModel/GreatAttack3.mv1", "GreatAtk3");

	pAnimator->Play(0);

	// 残像データ初期化
	for (int i = 0; i < AFTIMAGENUM; i++) {
		afterImagePos[i] = position;
		afterImageRotY[i] = rotation.y;
	}

	WeaponManager::GetInstance().LoadWeapons("Scr/Data/WeaponsData.json");
	maxWeaponId = WeaponManager::GetInstance().GetMaxWeaponId();
	changeWeaponButtonPressed = false;
	currentWeaponId = 10;
	WeaponData* weaponData = WeaponManager::GetInstance().GetWeapon(currentWeaponId);
	if (weaponData) {
		pWeapon = new Weapon(weaponData->name, weaponData->modelHandle);
		pWeapon->SetColLength(weaponData->colLength);
		pWeapon->SetColRadius(weaponData->colRadius);
		pWeapon->SetType((WeaponType)weaponData->type);
		pWeapon->SetAnimationSpeed(weaponData->attackSpeed);
		pWeapon->attach(modelHandle, pWeapon->GetModelHandle(), "wp", this);
	}

	playerAttack = new PlayerAttack(this, pWeapon);
	
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
	inputVec = VZero;

	//	移動入力
	MoveInput();

	////	攻撃入力・HitBox更新
	//AttackInput();

	playerAttack->Update();

	//	回避入力
	EvasionInput();

	//	BlinkのUpdate
	UpdateBlink();

	CheckWall();

	//	移動・アニメーション・回転処理
	UpdateMovement();

	//	斬撃更新
	//UpdateSlash();

	MV1SetMatrix(modelHandle, matrix);

	WeaponInput();

	//アイテムの取得
	AddItem();
	//アイテムのインベントリ表示非表示
	OpenInventory();

	//アイテムインベントリの更新
	if (isItemUI) {
		inventory.Update();
	}

	if (input->IsButtonDown(XINPUT_BUTTON_Y)) {
		SubHp(10);
	}

	pAnimator->Update();
	GameObject::Update();

	//	武器の更新
	if (pCollider != nullptr && pWeapon != nullptr) {
		pWeapon->Update();
	}

	if (pCollider != nullptr) {		pCollider->Update();
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
		PlayerStatusRender();
	}
#pragma endregion

#pragma region 残像描画処理
	// --- 残像描画 ---
	if (isBlinking && !playerAttack->IsAttacking()) {
		for (int i = AFTIMAGENUM - 1; i >= 0; i -= 4) {
			int alpha = 255 - 255 * i / AFTIMAGENUM;

			// モデルの座標・回転設定
			MATRIX matRot = MGetRotY(Deg2Rad(afterImageRotY[i]));
			MATRIX matTrans = MGetTranslate(afterImagePos[i]);
			MATRIX mat = MMult(matRot, matTrans);

			MV1SetMatrix(modelHandle, mat);

			// 半透明描画
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
			MV1DrawModel(modelHandle);
		}
	}
#pragma endregion

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
	if (pCollider != nullptr && pWeapon != nullptr) {
		pWeapon->Render();
	}
#pragma endregion
}

/// <summary>
/// 移動・アニメーション・回転処理
/// </summary>
void Player::UpdateMovement() {
	if (!playerAttack->IsAttacking()) {
		//	入力があれば
		if (VSquareSize(inputVec) >= 0.01f) {
			//	入力ベクトルの正規化
			inputVec = VNorm(inputVec);

			inputVec = VScale(inputVec, speed);

			inputVec = VScale(inputVec, evasionSpeed);

			//	カメラからみた移動する方向ベクトル
			VECTOR moveDirection = VZero;

			MATRIX mRotY = MGetRotY(Deg2Rad(Camera::main->GetRotation().y));		//	カメラのY軸回転行列
			moveDirection = VTransform(inputVec, mRotY);

			//	自身のy軸回転を計算した値に変更する
			rotation.y = Rad2Deg(atan2f(moveDirection.x, moveDirection.z)) + 180.0f;

			//	計算した入力ベクトル
			position = VAdd(position, VScale(moveDirection, 10.0f));

			//	移動アニメーションを再生
			if (evasionSpeed >= 1.2f) {
				pAnimator->Play(5, 0.5f);
			}
			else
				pAnimator->Play(1, 1.3f);
		}
		else {
			//	待機アニメーションを再生
			pAnimator->Play(0);
			evasionSpeed = 1;
		}
	}
}


/// <summary>
/// 移動入力
/// </summary>
void Player::MoveInput() {
	GetJoypadXInputState(DX_INPUT_PAD1, &XY);

	//入力処理
	//inputVec = VZero;
	inputVec = VGet(input->IsJoypadSthick("L_Horizontal"),
		0.0f,
		input->IsJoypadSthick("L_Vertical"));




	if (input->IsKey(KEY_INPUT_W))
		inputVec.z = inputVec.z + speed;
	if (input->IsKey(KEY_INPUT_S))
		inputVec.z = inputVec.z - speed;
	if (input->IsKey(KEY_INPUT_D))
		inputVec.x = inputVec.x + speed;
	if (input->IsKey(KEY_INPUT_A))
		inputVec.x = inputVec.x - speed;

	if (input->IsKey(KEY_INPUT_Q))
		inputVec = VAdd(inputVec, VUp);
	/*if (input->IsKey(KEY_INPUT_E))
		inputVec = VAdd(inputVec, VDown);*/
		/*if (input->IsKey(KEY_INPUT_E))
			inputVec = VAdd(inputVec, VDown);*/
}

/// <summary>
/// 回避入力
/// </summary>
void Player::EvasionInput() {
	// ===== 回避入力 =====
	bool isEvasionButtonDown = input->IsKeyDown(KEY_INPUT_LSHIFT) || InputManager::GetInstance()->IsButtonDown(XINPUT_BUTTON_A) || input->IsMouseDown(MOUSE_INPUT_MIDDLE);

	if (isEvasionButtonDown && !evasionButtonPressed && evasionCooldown <= 0.0f && VSize(inputVec) != 0) {
		// 押した瞬間＆クールダウン終了時のみ回避
		evasionButtonPressed = true;
		evasionCooldown = EVASION_COOLDOWN_TIME; // クールダウン開始
		Evasion();
	}
	else if (!isEvasionButtonDown) {
		// ボタンを離したら押下フラグリセット
		evasionButtonPressed = false;
	}

	// ===== 毎フレームクールダウン減算 =====
	if (evasionCooldown > 0.0f)
		evasionCooldown -= TimeManager::GetInstance()->deltaTime;
}

/// <summary>
/// 回避
/// </summary>
void Player::Evasion() {
	pCollider->SetEnable(false);

	// 瞬間移動
	evasionSpeed = 6;

	// 残像開始
	isBlinking = true;
	blinkTimer = 0.15f;

	// --- 履歴をすべて現在位置にリセット ---
	for (int i = 0; i < AFTIMAGENUM; i++) {
		afterImagePos[i] = position;
		afterImageRotY[i] = rotation.y;
	}
}

/// <summary>
/// BlinkのUpdate
/// </summary>
void Player::UpdateBlink() {
	// --- ブリンク中の処理 ---
	if (isBlinking && !playerAttack->IsAttacking()) {
		blinkTimer -= TimeManager::GetInstance()->deltaTime;   // 1フレーム経過（60FPS想定）
		if (blinkTimer <= 0.0f) {
			isBlinking = false;
			Dash();
			pCollider->SetEnable(true);
		}

		// 先に最新位置を入れる
		afterImagePos[0] = position;
		afterImageRotY[0] = rotation.y;

		// 古い残像を後ろにずらす
		for (int i = AFTIMAGENUM - 1; i > 0; i--) {
			afterImagePos[i] = afterImagePos[i - 1];
			afterImageRotY[i] = afterImageRotY[i - 1];
		}
	}
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
/// ダッシュ
/// </summary>
void Player::Dash() {
	evasionSpeed = 1.2f;
}

/// <summary>
/// 武器切り替え
/// </summary>
/// <param name="weaponId"></param>
void Player::ChangeWeapon(int weaponId) {
	// Weapon オブジェクトだけ削除
	if (pWeapon) {
		delete pWeapon;
		pWeapon = nullptr;
	}

	WeaponData* weaponData = WeaponManager::GetInstance().GetWeapon(weaponId);
	if (!weaponData) return;

	// 新しい Weapon を生成＆装備
	pWeapon = new Weapon(weaponData->name, weaponData->modelHandle);
	pWeapon->SetColLength(weaponData->colLength);
	pWeapon->SetColRadius(weaponData->colRadius);
	pWeapon->SetType((WeaponType)weaponData->type);
	pWeapon->SetAnimationSpeed(weaponData->attackSpeed);
	pWeapon->attach(modelHandle, pWeapon->GetModelHandle(), "wp", this);

	playerAttack->SetWeapon(pWeapon);

	// 必要なら Weapon Collider もセット
	pWeapon->SetCollider(new CapsuleCollider(pWeapon, VZero, VScale(VDown, 0), 8.0f));
}

/// <summary>
/// 武器切り替え入力
/// </summary>
void Player::WeaponInput() {
	if ((input->IsKeyDown(KEY_INPUT_C) && !changeWeaponButtonPressed ||
		input->IsButtonDown(XINPUT_BUTTON_BACK) && !changeWeaponButtonPressed) && !playerAttack->IsAttacking()) {
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
		if (hitItem && (input->IsKeyDown(KEY_INPUT_F) || input->IsButtonDown(XINPUT_BUTTON_B))) {
			item->SetVisible(false);
			std::string itemName = item->GetItem()->GetName();

			// インベントリへ追加
			GetInventory()->AddItem(std::move(item->TakeItem()));


			ItemDropManager::GetInstance()->RemoveItem(item.get());
			break; // erase後にvectorを操作しないようにbreak
		}
	}
}

/// <summary>
/// アイテムのインベントリを開く
/// </summary>
void Player::OpenInventory() {
	if (((input->IsKeyDown(KEY_INPUT_TAB)) || input->IsButtonDown(XINPUT_BUTTON_START)) && !isItemUI) {
		isItemUI = true;

	}
	else if (((input->IsKeyDown(KEY_INPUT_TAB)) || input->IsButtonDown(XINPUT_BUTTON_START)) && isItemUI) {
		isItemUI = false;

	}
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
		//	当たった相手を非表示にする
		//_pCol->GetGameObject()->SetVisible(false);
	/*	EffectManager::GetInstance()->Load("Res/Effect/01.efk", "FireFlower", 50.0f);
		Effect* pEffe = EffectManager::GetInstance()->Instantiate("FireFlower", position);

		Camera::main->Shake(2, 0.5f);*/

		//SceneManager::GetInstance()->SetNext(SceneType::GameOver);
		FadeManager::GetInstance()->FadeOut();
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
