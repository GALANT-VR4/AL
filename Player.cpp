#include "Player.h"
#include "ImGuiManager.h"
#include "Mathematics.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 playerPosition) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransForm_.Initialize();
	input_ = Input::GetInstance();
	worldTransForm_.translation_ += playerPosition;
}

void Player::Update() {
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	Vector3 move = {.0f, .0f, .0f};
	const float kCharacterSpeed = 0.2f;
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	}
	worldTransForm_.translation_ += move;
	if (worldTransForm_.translation_.y > 8.6)
		worldTransForm_.translation_.y = 8.6f;
	if (worldTransForm_.translation_.y < -8.6)
		worldTransForm_.translation_.y = -8.6f;
	if (worldTransForm_.translation_.x > 14.6)
		worldTransForm_.translation_.x = 14.6f;
	if (worldTransForm_.translation_.x < -14.6)
		worldTransForm_.translation_.x = -14.6f;
	worldTransForm_.matWorld_ = MakeAffineMatrix(
	    worldTransForm_.scale_, worldTransForm_.rotation_, worldTransForm_.translation_);
	Rotate();
	Attack();
	for (PlayerBullet* bullet : bullets_)
		bullet->Update();
	worldTransForm_.TransferMatrix();
	worldTransForm_.UpdateMatrix();
	ImGui::Begin("player.cpp update");
	ImGui::DragInt("score", &score_, .1f);
	ImGui::End();
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransForm_, viewProjection, textureHandle_);
	for (PlayerBullet* bullet : bullets_)
		bullet->Draw(viewProjection);
}

void Player::Rotate() {
	const float kRotSpd = .02f;
	if (input_->PushKey(DIK_A))
		worldTransForm_.rotation_.y -= kRotSpd;
	if (input_->PushKey(DIK_D))
		worldTransForm_.rotation_.y += kRotSpd;
	if (input_->PushKey(DIK_W))
		worldTransForm_.rotation_.x -= kRotSpd;
	if (input_->PushKey(DIK_S))
		worldTransForm_.rotation_.x += kRotSpd;
}

void Player::Attack() {
	fireTimer_--;
	if (input_->PushKey(DIK_SPACE) && fireTimer_ < 0) {
		fireTimer_ = 15;
		audio_->PlayWave(attackAudioHandle_);
		const float kBulletSpeed = 1.f;
		Vector3 velocity(0, 0, kBulletSpeed);
		velocity = TransformNomal(velocity, worldTransForm_.matWorld_);
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransForm_.translation_, velocity);
		bullet_ = newBullet;
		bullets_.push_back(newBullet);
	}
}

Player::~Player() {
	for (PlayerBullet* bullet : bullets_)
		delete bullet;
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransForm_.translation_.x;
	worldPos.y = worldTransForm_.translation_.y;
	worldPos.z = worldTransForm_.translation_.z;
	return worldPos;
}

void Player::OnCollision() {
	hp_--;
	audio_->PlayWave(hitAudiohandle_);
}

void Player::SetParent(const WorldTransform* parent) { worldTransForm_.parent_ = parent; }
