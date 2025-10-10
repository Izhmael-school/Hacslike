#include "TimeManager.h"
#include <DxLib.h>
#include "../Definition.h"
// 静的メンバ変数の初期化
TimeManager* TimeManager::pInstance = nullptr;

void TimeManager::CreateInstance() {
	pInstance = new TimeManager();
}

TimeManager* TimeManager::GetInstance() {
	if (pInstance == nullptr)
		CreateInstance();

	return pInstance;
}

void TimeManager::DestroyInstance() {
	if (pInstance != nullptr) {
		delete pInstance;
		pInstance = nullptr;
	}
}

TimeManager::TimeManager() 
	:prev()
	,current()
	,m()
	,s()
	,ms()
	,deltaTime(0)
{
	Start();
}

void TimeManager::Start() {
	// 起動時の時間を取得
	prev = GetNowCount();
	current = prev;
}

void TimeManager::Update() {
	// 今の時間を取得
	current = GetNowCount();
	// 前回のフレームとの差を計算
	deltaTime = current - prev;
	// 経過時間の加算
	ms += deltaTime;
	// Unityみたいに値で使うように変換
	deltaTime /= 1000.0f;

	// ミリ秒から秒に変換
	if (ms >= 1000) {
		ms -= 1000;
		s += 1;
	}

	if (s >= 60.0f) {
		s -= 60.0f;
		m++;
	}

	prev = current;
}

void TimeManager::Render() {
	DrawFormatString(100, 100, red, "%02d %d",deltaTime,ms);
}
