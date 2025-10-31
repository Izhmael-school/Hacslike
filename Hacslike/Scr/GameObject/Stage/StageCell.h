#pragma once
#include "../GameObject.h"
#include "../../Definition.h"
#include "../../Component/Collider/Collider.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/StageManager.h"

class StageCell : public GameObject {
private:
	int modelHandle;
	ObjectType type;
	bool isTouch;

public:
	StageCell(int _modelHandle, ObjectType _type,VECTOR position);
	~StageCell();

	virtual void Start() override;
	virtual void Update() override;
	virtual void Render() override;

	/*
 * @function	GetModelHandle
 * @brief		モデルハンドルの取得
 * @return		int
 */
	inline int GetModelHandle() const { return modelHandle; }

	/*
	 * @function	SetModelHandle
	 * @brief		モデルハンドルの変更
	 * @param[in]	int _v
	 */
	inline void SetModelHandle(int& _v) { modelHandle = _v; }
	inline void SetModelHandleDup(int _v) { modelHandle = _v; }

	inline ObjectType GetObjectType() const { return type; }
	void OnTriggerEnter(Collider* _pOther) override;
	void OnTriggerExit(Collider* _pOther) override;

};

