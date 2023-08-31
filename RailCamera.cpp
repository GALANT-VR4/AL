#include "RailCamera.h"
#include "Mathematics.h"
#include "ImGuiManager.h"

void RailCamera::Initialize(Vector3 trans, Vector3 rot) {
	worldTransform_.Initialize();
	worldTransform_.translation_ = trans;
	worldTransform_.rotation_ = rot;
	viewProjection_.Initialize();
}

void RailCamera::Update() {
	Vector3 move = {0.f, -0.f, 0.f};
	Vector3 rot = {0.f, 0.f, 0.f};

	worldTransform_.translation_ += move;
	worldTransform_.rotation_ += rot;

	worldTransform_.UpdateMatrix();

	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	viewProjection_.TransferMatrix();
	

}
