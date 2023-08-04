#include "Player.h"
#include "Mathematics.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransForm_.Initialize();
	input_ = Input::GetInstance();
}

void Player::Update() {
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
	worldTransForm_.matWorld_ = MakeAffineMatrix(
	    worldTransForm_.scale_, worldTransForm_.rotation_, worldTransForm_.translation_);
	Rotate();
	Attack();
	for (PlayerBullet* bullet : bullets_)
		bullet->Update();
	worldTransForm_.TransferMatrix();
	worldTransForm_.UpdateMatrix();
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
}

void Player::Attack() {
	if (input_->PushKey(DIK_SPACE)) {
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransForm_.translation_);
		bullet_ = newBullet;
		bullets_.push_back(newBullet);
	}
}

Player::~Player() {
	for (PlayerBullet* bullet : bullets_)
		delete bullet;
}
