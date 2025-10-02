#pragma once
#include "../GameObject.h"
#include "../../../Definition.h"

class StageCell : public GameObject {
private:
	int modelHandle;
	CellState state;

public:
	StageCell(int _modelHandle,CellState _state);
	~StageCell();

	virtual void Start() override;
	virtual void Update() override;
	virtual void Render() override;
};

