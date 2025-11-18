#pragma once

enum class GameStatus {
	Playing,
	Stop,

};

class GameSystem
{
public:
	static GameSystem* pInstance;
	GameStatus currentGameStatus;
private://静的メンバ関数
	/// <summary>
	/// CreateInstance
	/// 自信のインスタンスを生成する
	/// </summary>
	static void CreateInstance();

public://静的メンバ関数
	/// <summary>
	/// GetInstance
	/// 自身のインスタンスを取得する唯一の手段
	/// return InputManager*
	/// </summary>
	/// <returns></returns>DINPUT_JOYSTATE
	static GameSystem* GetInstance();

	/// <summary>
	/// DestroyInstance
	/// 自身のインスタンスを破棄する唯一の手段
	/// </summary>
	static void DestroyInstance();
public:
	GameSystem();

	~GameSystem();

public:
	inline const void SetGameStatus(GameStatus newStatus) { currentGameStatus = newStatus; }

	inline const GameStatus GetGameStatus() { return currentGameStatus; }
	
	inline const bool IsPlayable() { return currentGameStatus == GameStatus::Playing; }

	inline const bool IsStop() { return currentGameStatus == GameStatus::Stop; }



};

