#pragma once
//	=================================================================
//		ヘッダーをインクルードする場所
//	=================================================================
#include <DxLib.h>
#include <string>

//	=================================================================
//		定数を定義する場所
//	=================================================================
#define WINDOW_WIDTH			(1200)	//	ウィンドウの横幅
#define WINDOW_HEIGHT			(800)	//	ウィンドウの縦幅

#define FPS						(60)	//	FPS

//	引数付きマクロ定義 変換マクロ
#define Deg2Rad(x) ( x * (DX_PI_F / 180.0f )) //　デグリー角→ラジアン角
#define Rad2Deg(x) ( x * (180.0f / DX_PI_F )) //  ラジアン角→デグリー角

// ワールドベクトル
#define VRight	 (VGet(1.0f,0,0))
#define VLeft	 (VGet(-1.0f,0,0))
#define VUp		 (VGet(0,1.0f,0))
#define VDown	 (VGet(0,-1.0f,0))
#define VForward (VGet(0,0,1.0f))
#define VBack    (VGet(0,0,-1.0f))

// ゼロベクトル
#define VZero   (VGet(0,0,0))
#define VOne   (VGet(1,1,1))

// DxLibの読み込みエラ-
#define INVALID (-1)

#define EnemyMax (10)

#define PLAYER_MODEL_HANDLE (MV1LoadModel("Res/PlayerModel/Neutral.mv1"))
//	=================================================================
//		定数を定義する場所
//	=================================================================
//	色
const unsigned int red = GetColor(255, 0, 0);
const unsigned int green = GetColor(0, 255, 0);
const unsigned int blue = GetColor(0, 0, 255);
const unsigned int magenta = GetColor(255, 0, 255);
const unsigned int cyan = GetColor(0, 255, 255);
const unsigned int yellow = GetColor(255, 255, 0);
const unsigned int white = GetColor(255, 255, 255);
const unsigned int gray = GetColor(120, 120, 120);
const unsigned int black = GetColor(0, 0, 0);

enum class SceneType {
	Title,
	Game,
	Clear,
	HowTo,

	Sekino,
	Saito,
	Tanaka,
};

enum class FadeState {
	FadeIn = -1,
	FadeOut = 1,
	FadeEnd,
};

enum class CellState {
	INVAILD,
	None,
	Stair
};
