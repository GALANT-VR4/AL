#include "PlayerBullet.h"
#include "ImGuiManager.h"
#include <cassert>
void PlayerBullet::Initialize(Model* model, const Vector3& position) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("white1x1.png");
	worldTransForm_.Initialize();
	worldTransForm_.translation_ = position;
}

void PlayerBullet::Update() {
	worldTransForm_.TransferMatrix();
	worldTransForm_.UpdateMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	
	model_->Draw(worldTransForm_, viewProjection, textureHandle_);
}
