#include "ImGuiManager.h"
#include "EnemyBullet.h"
#include <cassert>
void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("cube/cube.jpg");
	worldTransForm_.Initialize();
	worldTransForm_.translation_ = position;
	velocity_ = velocity;
}

void EnemyBullet::Update() {
	worldTransForm_.TransferMatrix();
	worldTransForm_.translation_ += velocity_;
	worldTransForm_.UpdateMatrix();
	if (--deathTimer_ <= 0)
		isDead_ = true;
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransForm_, viewProjection, textureHandle_);
}

void EnemyBullet::OnCollision() { isDead_ = true; }

Vector3 EnemyBullet::GetWorldPosition() { return worldTransForm_.translation_; }
