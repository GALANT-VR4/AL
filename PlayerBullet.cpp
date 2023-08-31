#include "PlayerBullet.h"
#include "ImGuiManager.h"
#include <cassert>
void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("white1x1.png");
	worldTransForm_.Initialize();
	worldTransForm_.translation_ = position;
	velocity_ = velocity;
}

void PlayerBullet::Update() {
	worldTransForm_.TransferMatrix();
	worldTransForm_.translation_ += velocity_;
	worldTransForm_.UpdateMatrix();
	if (--deathTimer_ <= 0)
		isDead_ = true;
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransForm_, viewProjection, textureHandle_);
}

void PlayerBullet::OnCollision() { isDead_ = true; }

Vector3 PlayerBullet::GetWorldPosition() { return worldTransForm_.translation_; }

// void PlayerBullet::SetParent(const WorldTransform* parent) { worldTransForm_.parent_ = parent; }
