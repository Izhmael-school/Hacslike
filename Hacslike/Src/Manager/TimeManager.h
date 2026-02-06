#pragma once
#include "../Component/Singleton.h"

class TimeManager : public Singleton<TimeManager> {
private:
	int prev;
	int current;
	int m = 0, s = 0, ms = 0;

public:
	float deltaTime;

public:
	TimeManager();
	~TimeManager() = default;
	void Start();
	void Update();
	void Render();

	inline int GetCurrent() const { return current; }
};

