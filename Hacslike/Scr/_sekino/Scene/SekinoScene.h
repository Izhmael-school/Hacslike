#pragma once
#include "BaseScene.h"
class SekinoScene : public BaseScene {
public:
	 SekinoScene();
	 ~SekinoScene();


	virtual void Start() override;
	virtual void Update() override;
	virtual void Render() override;
};

