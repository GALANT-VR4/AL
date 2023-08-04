#include "Enemy.h"
#include <cassert>

void Enemy::Initialize(Model* model) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("tex1.png");
	worldTransForm_.Initialize();
	worldTransForm_.translation_ = {.0f, .0f, 50.f};
}

void Enemy::Update() {

	worldTransForm_.TransferMatrix();
	switch (phase_) { 
	case Phase::Approach:
	default:
		worldTransForm_.translation_ += {.0f,.0f,-.5f};
		if (worldTransForm_.translation_.z < 0)
			phase_ = Phase::Leave;
		break;
	case Phase::Leave:
		worldTransForm_.translation_ += {.5f, .5f, -.5f};
		break;
	}
	worldTransForm_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransForm_, viewProjection, textureHandle_);
}
