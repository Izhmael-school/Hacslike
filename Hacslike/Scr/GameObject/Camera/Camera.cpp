#include "Camera.h"
#include "../../Manager/InputManager.h"

Camera* Camera::main = nullptr;

Camera::Camera(VECTOR _pos, float _length) 
	:GameObject(_pos)
	,pTarget(nullptr)
	,armLenght(_length)
	, XY()
	, offSet(VGet(0.0f, 700.0f, -400.0f))
	, rotation_X(70)
{
	Start();
	main = this;
}

Camera::~Camera() {}

void Camera::Start() {
	rotation.x += rotation_X;
}

void Camera::Update() {
	//中心点が原点で半径1の球面上の座標
	VECTOR sphere = VGet(
		sinf(Deg2Rad(rotation.x - 90)) * sinf(Deg2Rad(rotation.y)),
		cosf(Deg2Rad(rotation.x - 90)),
		sinf(Deg2Rad(rotation.x - 90)) * cosf(Deg2Rad(rotation.y)));

	//座標系や回転、三角関数の相互関係等を組み込んだ最終形態
	sphere = VGet(
		-cosf(Deg2Rad(rotation.x)) * sinf(Deg2Rad(rotation.y)),
		sinf(Deg2Rad(rotation.x)),
		-cosf(Deg2Rad(rotation.x)) * cosf(Deg2Rad(rotation.y)));

	//半径をarmLength倍
	sphere = VScale(sphere, armLenght);
	//中心点を追従対象の座標分平行移動
	sphere = VAdd(sphere, pTarget->GetPosition());


	//最終的な計算結果を自身の座標に適応する
	position = sphere;



	GameObject::Update();

	//カメラの位置と回転を設定する
	SetCameraPositionAndAngle(VAdd(position, offSet), Deg2Rad(rotation.x), Deg2Rad(rotation.y), Deg2Rad(rotation.z));
	//リスナーの設定
	Set3DSoundListenerPosAndFrontPos_UpVecY(position, VAdd(position, forward));
}

void Camera::Render() {}
