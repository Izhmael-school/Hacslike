#include "Camera.h"
#include "../../Manager/InputManager.h"

Camera* Camera::main = nullptr;

Camera::Camera(VECTOR _pos, float _length) 
	:GameObject(_pos)
	,pTarget(nullptr)
	,armLenght(_length)
	, XY()
{
	Start();
	main = this;
}

Camera::~Camera() {}

void Camera::Start() {}

void Camera::Update() {
	// 入力管理クラスの取得
	InputManager* input = InputManager::GetInstance();

	// 入力ベクトル
	VECTOR inputVec = VZero;

	GetJoypadXInputState(DX_INPUT_PAD1,&XY);

	if (XY.ThumbRX >= 1000)
		inputVec = VAdd(inputVec, VScale(VLeft, 4.0f));

	if (XY.ThumbRX <= -1000)
		inputVec = VAdd(inputVec, VScale(VRight, 4.0f));

	if (XY.ThumbRY <= -1000)
		inputVec = VAdd(inputVec, VScale(VUp, 4.0f));

	if (XY.ThumbRY >= 1000)
		inputVec = VAdd(inputVec, VScale(VDown, 4.0f));

	// 入力ベクトルの正規化
	if (VSquareSize(inputVec) >= 0.01f) {
		inputVec = VNorm(inputVec);
	}

	// 最終的な入力ベクトルを自身の回転（オイラー角）に加える
	rotation.y += -inputVec.x;	// y軸回転
	if (rotation.x + inputVec.y > !91) {
		rotation.x += inputVec.y;	// x軸回転
		if (rotation.x >= 91) {
			rotation.x = 90;
		}
		if (rotation.x<= -1) {
			rotation.x = 0;
		}
	}

	//if (rotation.x + inputVec.x <! -91) {
	//	rotation.x += inputVec.y;	// x軸回転
	//}


	

	// 半径1の球面上の座標
	VECTOR sphere = VGet(
		sinf(Deg2Rad(rotation.x)) * sinf(Deg2Rad(rotation.y)),
		cosf(Deg2Rad(rotation.x)),
		sinf(Deg2Rad(rotation.x)) * cosf(Deg2Rad(rotation.y))
	);

	// 座標系や、回転、三角関数の相互の関係等を組み込んだ最終形態
	sphere = VGet(
		-cosf(Deg2Rad(rotation.x)) * sinf(Deg2Rad(rotation.y)),
		sinf(Deg2Rad(rotation.x)),
		-cosf(Deg2Rad(rotation.x)) * cosf(Deg2Rad(rotation.y))
	);

	// 半径を armLenght倍する
	sphere = VScale(sphere, armLenght);

	// 中心点を中心対象の座標分、平行移動
	sphere = VAdd(sphere, pTarget->GetPosition());


	position = sphere;

	// カメラの位置と回転を設定する
	SetCameraPositionAndAngle(position, Deg2Rad(rotation.x), Deg2Rad(rotation.y), Deg2Rad(rotation.z));
}

void Camera::Render() {}
