#pragma once
#include "BaseScene.h"
class GameScene : public BaseScene {
private:

public:
	GameScene();
	~GameScene();


	virtual void Start() override;
	virtual void Update() override;
	virtual void Render() override;
};

