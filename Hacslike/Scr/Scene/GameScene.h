#pragma once
#include "BaseScene.h"
#include"../Manager/SkillManager.h"
#include"../Skill/SkillSelectUI.h"
class GameScene : public BaseScene {
protected:
	SkillSelectUI skillUI;
	std::vector<std::shared_ptr<Skill>> skillChoices;
	bool isSelectingSkill = false;
	
private:

public:
	GameScene();
	~GameScene();


	virtual void Start() override;
	virtual void Update() override;
	virtual void Render() override;
};

