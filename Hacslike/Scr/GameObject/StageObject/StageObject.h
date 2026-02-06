//#pragma once
//#include "../GameObject.h"
//
//enum MenuOpenState {
//	Invalid = -1,
//	Open,
//	Close
//};
//
//class StageObject : public GameObject {
//public:
//	StageObject(int _mHandle);
//	~StageObject();
//
//	void Start() override;
//	void Update() override;
//	void Render() override;
//	void OnTriggerEnter(Collider* _pOther) override;
//	void OnTriggerExit(Collider* _pOther) override;
//
//protected:
//	virtual void OpenExecute();
//	virtual void CloseExecute();
//
//	virtual void Setup();
//	virtual void Teardown();
//protected:
//	int modelHandle;
//	bool isHitPlayer;
//	MenuOpenState menuOpenState;
//};
//
