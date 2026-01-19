#pragma once
#include "../Enemy.h"
#include "../../../Artifact/ArtifactSelectUI.h"

class BossBase : public Enemy {
public:
	ArtifactSelectUI artifactSelectUI;
	std::vector<std::shared_ptr<ArtifactBase>> artifactChioces;
	bool isSelectArtifact = false;
protected:
	BossBase();
	~BossBase();

protected:
	void AppearStair();

public:
public:
	void Start() override;
	void Update() override;
	void Render() override;

	void DeadExecute() override;


private:



	// äKíiÇ™èoåªÇ∑ÇÈà íu
	VECTOR appearPos;

public:
	inline void SetAppearPos(VECTOR pos) { appearPos = pos; }
};

