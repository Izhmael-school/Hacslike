#pragma once
#include "../Definition.h"
#include "../Component/Singleton.h"

class SceneManager : public Singleton<SceneManager> {

private:
	class BaseScene* pCurrentScene;	// ç°ÇÃÉVÅ[Éì
	SceneType Current;
	SceneType next;
	bool changed;

public:
	SceneManager();
	~SceneManager() = default;

	void Update();

	void Render();

	void LoadScene();

public:
	void SetNext(SceneType _next);

};

