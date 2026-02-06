#pragma once
#include "../GameObject.h"
#include "../../Component/Collider/Collider.h"

class SphereHitBox;

class TitleReturner : public GameObject {
public:
	static TitleReturner* pInstance;

private:
	int modelHandle;
	Collider* pCollider;
	bool canReturner;
	
	bool isVisible;

	bool isShowResetUI = false;

	bool ShowResetUI;       // UI表示フラグ
	float resetUITimer;       // 表示時間カウント用変数
	float resetUIDuration = 5.0f;  // UI表示時間（3秒）

private:
	/// <summary>
	/// 自身のインスタンスを生成する
	/// </summary>
	static void CreateInstance();

public:
	/// <summary>
	/// 自身のインスタンスを取得
	/// </summary>
	/// <returns></returns>
	static TitleReturner* GetInstance();

	/// <summary>
	/// 自身のインスタンスを破棄
	/// </summary>
	static void DestroyInstance();

public:
	TitleReturner();
	virtual ~TitleReturner();

	void Start() override;
	void Update() override;
	void Render() override;

	void ShowFloorResetUI();

	inline void SetisShowResetUI(bool _s) { isShowResetUI = _s; }

	inline bool GetisShowResetUI() { return isShowResetUI; }

	void OnTriggerStay(Collider* _pCol) override;

	void OnTriggerExit(Collider* _pCol) override;


	inline void SetVisible(bool _v) { isVisible = _v;};

};

