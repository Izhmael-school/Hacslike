#include "RigidBody.h"
#include "../GameObject/GameObject.h"

RigidBody* RigidBody::pInstance = nullptr;


RigidBody::RigidBody()
{}

RigidBody::~RigidBody() {

}

void RigidBody::CreateInstance() {
	pInstance = new RigidBody();
}

RigidBody* RigidBody::GetInstance() {
	if (pInstance == nullptr)
		CreateInstance();

	return pInstance;
}

void RigidBody::DestroyInstance() {
	if (pInstance != nullptr) {
		delete pInstance;
		pInstance = nullptr;
	}
}

void RigidBody::Update(GameObject* pObj) {
	
	if (!pObj->IsVisible())
		return;


	pObj->SetPosition(VGet(pObj->GetPosition().x, pObj->GetPosition().y - 1, pObj->GetPosition().z));

	
}
