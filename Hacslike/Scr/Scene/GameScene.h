#pragma once
#include "BaseScene.h"
#include"../Manager/SkillManager.h"
#include"../Skill/SkillSelectUI.h"
#include "../GameObject/Artifact/ArtifactSelectUI.h"
class GameScene : public BaseScene {
protected:
	SkillSelectUI skillUI;
	std::vector<std::shared_ptr<Skill>> skillChoices;
	bool isSelectingSkill = false;
	ArtifactSelectUI artifactUI;
	std::vector<std::shared_ptr<ArtifactBase>> artifactChioces;
	bool isSelectingArtifact = false;
	
private:

public:
	GameScene();
	~GameScene();


	virtual void Start() override;
	virtual void Update() override;
	virtual void Render() override;
};

