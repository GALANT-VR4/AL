#include "Enemy.h"
#include "Mathematics.h"
#include <cassert>

void Enemy::Initialize(Model* model) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("tex1.png");
	worldTransForm_.Initialize();
	worldTransForm_.translation_ = {.0f, .0f, 50.f};

	AppInit();
}

void Enemy::Update() {
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	worldTransForm_.TransferMatrix();
	switch (phase_) { 
	case Phase::Approach:
	default:
		worldTransForm_.translation_ += {.0f,.0f,-.1f};
		if (worldTransForm_.translation_.z < 0)
			phase_ = Phase::Leave;
		fireTimer--;
		if (fireTimer == 0) {
			Fire();
			fireTimer = kFireInterval;
		}
		break;
	case Phase::Leave:
		worldTransForm_.translation_ += {.5f, .5f, -.1f};
		break;
	}
	worldTransForm_.UpdateMatrix();
	for (EnemyBullet* bullet : bullets_)
		bullet->Update();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransForm_, viewProjection, textureHandle_);
	for (EnemyBullet* bullet : bullets_)
		bullet->Draw(viewProjection);
}

void Enemy::Fire() {
	const float kBulletSpeed = 1.f;
	Vector3 velocity(0, 0, -kBulletSpeed);
	velocity = TransformNomal(velocity, worldTransForm_.matWorld_);
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransForm_.translation_, velocity);
	bullet_ = newBullet;
	bullets_.push_back(bullet_);
}

Enemy::~Enemy() {
	for (EnemyBullet* bullet : bullets_)
		delete bullet;
}

void Enemy::AppInit() { fireTimer = kFireInterval; }
