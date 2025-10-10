#pragma once
#include "../GameObject.h"
#include "../../../Definition.h"

class StageCell : public GameObject {
private:
	int modelHandle;
	ObjectType type;

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
};

