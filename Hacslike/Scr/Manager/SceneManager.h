#pragma once
#include "../Definition.h"
#include "../Component/Singleton.h"

class SceneManager : public Singleton<SceneManager> {

private:
	class BaseScene* pCurrentScene;	// 今のシーン
	class BaseScene* pSceneList[(int)SceneType::Max];

private:
	void Start();
public:
	SceneManager();
	~SceneManager();

	/// <summary>
	/// シーンの更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// シーンの描画処理
	/// </summary>
	void Render();

	/// <summary>
	/// シーンの変更
	/// </summary>
	/// <param name="_next">変更するシーン</param>
	void ChangeScene(SceneType _next);
};

