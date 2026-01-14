#pragma once
#include "BaseScene.h"
#include <List>
class SekinoScene : public BaseScene {
private:

public:
	 SekinoScene();
	 ~SekinoScene();


	virtual void Start() override;
	virtual void Update() override;
	virtual void Render() override;

	void Setup() override;
	void Teardown() override;
};

