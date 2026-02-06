#pragma once
#include "BaseScene.h"
#include"../Manager/SkillManager.h"
#include"../Skill/SkillSelectUI.h"
#include "../GameObject/Artifact/ArtifactSelectUI.h"
#include "../Enhancement/StatusEnhancement.h"
#include "../GameObject/Character/Player/Player.h"
#include"../Manager/SalesManager.h"

class GameScene : public BaseScene {
protected:
	SkillSelectUI skillUI;
	std::vector<std::shared_ptr<Skill>> skillChoices;
	bool isSelectingSkill = false;
	ArtifactSelectUI artifactUI;
	std::vector<std::shared_ptr<ArtifactBase>> artifactChioces;
	bool isSelectingArtifact = false;

	StatusEnhancement statusEnhancement;

	SalesManager salesManager;

	bool isEnhancing = false; // 最初は表示状態からスタート

	Player* pPlayer;

private:

public:
	GameScene();
	~GameScene();

	virtual void Start() override;
	virtual void Update() override;
	virtual void Render() override;
	void Setup() override;
	void Teardown() override;
};

