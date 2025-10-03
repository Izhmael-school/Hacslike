#include "Player.h"
#include "../Hacslike/Scr/_sekino/Manager/InputManager.h"
#include "../Hacslike/Scr/_sekino/Manager/SceneManager.h"
#include "../Hacslike/Scr/_sekino/Manager/FadeManager.h"
#include "../Hacslike/Scr/_sekino/GameObject/Camera/Camera.h"
#include "../Hacslike/Scr/_sekino/Component/Collider.h"
//#include "../Hacslike/Scr/_sekino/Manager/EffectManager.h"
//#include "../Hacslike/Scr/_sekino/Manager/Effect/Effect.h"


/*
 *	@brief		コンストラクタ
 *	@param[in]	VECTOR _pos		初期化する座標
 */
Player::Player(VECTOR _pos)
	: Character(_pos, "Player")
	, isAttacking(false)
	, pWeapon(nullptr)
	, XY() {
	Start();
}

/*
 *	@breif		デストラクタ
 */
Player::~Player() {
	delete pWeapon;
	pWeapon = nullptr;
}

/*
 *	@function	Start
 *	@breif		初期化処理
 */
void Player::Start() {
	//	非表示だったら初期化しない
	if (!isVisible)
		return;

	modelHandle = MV1LoadModel("Res/PlayerModel/Neutral.mv1");

	pAnimator->SetModelHandle(modelHandle);

	pAnimator->Load("Res/PlayerModel/Neutral.mv1", true);
	pAnimator->Load("Res/PlayerModel/Run.mv1", true);
	pAnimator->Load("Res/PlayerModel/Attack1.mv1");

	pAnimator->Play(0);

	////	Z軸方向を正面に向かせる
	//rotation.y = 180;

}

/*
 *	@function	Update
 *	@breif		更新処理
 */
void Player::Update() {
	//	非表示だったら更新しない
	if (!isVisible)
		return;

	//	入力管理クラスの取得
	InputManager* input = InputManager::GetInstance();

	//	入力ベクトル
	VECTOR inputVec = VZero;

	GetJoypadXInputState(DX_INPUT_PAD1, &XY);

	if (XY.ThumbLY >= 1000 || input->IsKey(KEY_INPUT_W))
		inputVec = VAdd(inputVec, VForward);
	if (XY.ThumbLY <= -1000 || input->IsKey(KEY_INPUT_S))
		inputVec = VAdd(inputVec, VBack);
	if (XY.ThumbLX >= 1000 || input->IsKey(KEY_INPUT_D))
		inputVec = VAdd(inputVec, VRight);
	if (XY.ThumbLX <= -1000 || input->IsKey(KEY_INPUT_A))
		inputVec = VAdd(inputVec, VLeft);
	if (input->IsKey(KEY_INPUT_Q))
		inputVec = VAdd(inputVec, VUp);
	if (input->IsKey(KEY_INPUT_E))
		inputVec = VAdd(inputVec, VDown);

	if (input->IsKeyDown(KEY_INPUT_SPACE)) {
		isAttacking = true;
		pAnimator->Play(2);
	}
	if (pAnimator->GetCurrentAnimation() != 2) {
		isAttacking = false;
	}

	if (!isAttacking) {
		//	入力があれば
		if (VSquareSize(inputVec) >= 0.01f) {
			//	入力ベクトルの正規化
			inputVec = VNorm(inputVec);

			//	カメラからみた移動する方向ベクトル
			VECTOR moveDirection = VZero;

#if 0	//	三角関数で計算する場合

			//	XZ平面の回転として考える ->	2次元の回転として扱える
			//	回転後の座標を X, Z 、回転前の座標を x, z としたとき
			//	X = x * cosθ - z * sinθ
			//	Z = x * sinθ + z * cosθ
			//	θ = カメラのy軸回転 -> camera.rotation.y]

			float theta = Deg2Rad(Camera::main->GetRotation().y);
			//	座標系の違いから X, Z の値は入れ替える
			float Z = inputVec.z * cosf(theta) - inputVec.x * sinf(theta);
			float X = inputVec.z * sinf(theta) + inputVec.x * cosf(theta);

			//	移動方向を計算結果で初期化
			moveDirection = VGet(X, 0, Z);
			{
				//	XZ平面の原点から見た傾き(tanθ)を求める
				float tanTheta = X / Z;
				//	tanθ から θだけを求める -> 逆三角関数を使う
				atan(tanTheta);
			}

			//	tanθ -> θを出す

			//	自身のy軸回転を計算した値に変更する
			rotation.y = Rad2Deg(atan2f(X, Z)) + 180.0f;

#else	//	行列で計算する場合

			//	DxLibにおいて行列の型が MATRIX型
			//	４行４列 で 要素は float型
			////	カメラの回転行列を取得する
			//float thetaY = Deg2Rad(Camera::main->GetRotation().y);
			//float thetaX = Deg2Rad(Camera::main->GetRotation().x);
			//float thetaZ = Deg2Rad(Camera::main->GetRotation().z);

			//MATRIX mRotX = MGetRotX(thetaX);		//	カメラのX軸回転行列
			//MATRIX mRotZ = MGetRotZ(thetaZ);		//	カメラのZ軸回転行列

			////	X->Y->Z の順で回転行列を作成する
			//MATRIX mRotXYZ = MMult(MMult(mRotX, mRotY), mRotZ);

			////	拡縮行列を取得する
			//MATRIX mScale = MGetScale(scale);

			////	平行移動行列を取得する
			//MATRIX mTranslate = MGetTranslate(position);

			////	行列の乗算は合成
			////	回転行列 -> 拡縮行列 -> 平行行列 の順に掛け合わせる
			//MATRIX matrix = MMult(MMult(mRotXYZ, mScale), mTranslate);
			//	これが Unity, UnrealEngine, 3Dゲームプログラミングで利用される 4*4の行列
			MATRIX mRotY = MGetRotY(Deg2Rad(Camera::main->GetRotation().y));		//	カメラのY軸回転行列
			moveDirection = VTransform(inputVec, mRotY);

			//	移動方向を計算結果で初期化



			//	自身のy軸回転を計算した値に変更する
			rotation.y = Rad2Deg(atan2f(moveDirection.x, moveDirection.z)) + 180.0f;

#endif

			//	計算した入力ベクトル
			position = VAdd(position, VScale(moveDirection, 10.0f));

			//	移動アニメーションを再生
			pAnimator->Play(1, 0.2f);
		}
		else {
			//	待機アニメーションを再生
			pAnimator->Play(0);
		}
	}

	pAnimator->Update();
	GameObject::Update();

	////計算した座標、回転(オイラー角)、拡縮モデルに反映する
	//MV1SetPosition(modelHandle, position);
	//MV1SetRotationXYZ(modelHandle, VScale(rotation, DX_PI_F / 180.0f));
	//MV1SetScale(modelHandle, scale);
	MV1SetMatrix(modelHandle, matrix);

	//	武器の更新
	if (pCollider != nullptr) {
		pWeapon->Update();
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

	//	モデルの描画
	MV1DrawModel(modelHandle);

	//	武器の描画
	if (pCollider != nullptr)
		pWeapon->Render();
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

/*
 *	@function	OnTriggerSaty
 *	@brief		当たっている間
 *	@param[in]	Collider* _pCol
 */
void Player::OnTriggerStay(Collider* _pCol) {
}

/*
 *	@function	OnTriggerExit
 *	@brief		離れた瞬間
 *	@param[in]	Collider* _pCol
 */
void Player::OnTriggerExit(Collider* _pCol) {
}
