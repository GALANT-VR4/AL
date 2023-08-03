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
	worldTransForm_.TransferMatrix();
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
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransForm_, viewProjection, textureHandle_);
}
