#pragma once
#include "../Definition.h"
#include "../Component/Singleton.h"

class SceneManager : public Singleton<SceneManager> {

private:
	class BaseScene* pCurrentScene;	// 今のシーン
	class BaseScene* pSceneList[(int)SceneType::Max];

	bool isEnd;
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


	inline bool GetEnd() const { return isEnd; }

	inline void SetEnd(bool _End) { isEnd = _End; }
};

