#include "Enemy.h"
#include "ImGuiManager.h"
#include "Mathematics.h"
#include "Player.h"
#include <cassert>
#include <stdlib.h>
#include <time.h>

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
	srand((unsigned int)clock());
	worldTransForm_.translation_ = {float(rand() % 104) - 52, float(rand() % 58) - 29, 50.f};
	switch (phase_) {
	case Phase::Approach:
	default:
		worldTransForm_.translation_ += {.0f, .0f, -.0f};
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
	//model_->Draw(worldTransForm_, viewProjection, textureHandle_);
	for (EnemyBullet* bullet : bullets_)
		bullet->Draw(viewProjection);
}

void Enemy::Fire() {
	const float kBulletSpeed = .05f;

	Vector3 velocity =
	    TransformNomal(
	        (player_->GetWorldPosition() - GetWorldposition()), worldTransForm_.matWorld_) *
	    kBulletSpeed;
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

Vector3 Enemy::GetWorldposition() {
	Vector3 worldPos;
	worldPos.x = worldTransForm_.translation_.x;
	worldPos.y = worldTransForm_.translation_.y;
	worldPos.z = worldTransForm_.translation_.z;
	return worldPos;
}

void Enemy::onCollision() {}
