#pragma once
#include "../GameObject.h"
#include "../../UI/MenuSaveLoad.h"
class SaveObject : public GameObject
{
public:
	static SaveObject* pInstance;

private:
	int modelHandle;
	Collider* pCol;
	bool hitObject;
	bool openSaveMenu;
	// セーブメニュー表示用ポインタ（インベントリやアーティファクトと同様に管理する）
	MenuSaveLoad* pSaveMenu = nullptr;
public:
	SaveObject(VECTOR _pos);
	~SaveObject();

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
	static SaveObject* GetInstance();

	/// <summary>
	/// DestroyInstance
	/// 自身のインスタンスを破棄する唯一の手段
	/// </summary>
	static void DestroyInstance();

	void Start() override;
	void Update() override;
	void Render() override;

	inline bool GetIsOpenSaveMenu() { return openSaveMenu; }

	void OnTriggerStay(Collider* _pCol) override;

	void OnTriggerExit(Collider* _pCol) override;
};

