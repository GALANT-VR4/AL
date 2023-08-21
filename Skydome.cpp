#include "Skydome.h"

void Skydome::Initialize(Model* model) {
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = {5, 5, 5};
}

void Skydome::Update() {}

void Skydome::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}
