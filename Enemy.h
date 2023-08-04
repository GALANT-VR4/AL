#pragma once
#include "Model.h"
#include "WorldTransform.h"
enum class Phase {
	Approach,
	Leave,
};
class Enemy {
public:
	void Initialize(Model* model);
	void Update();
	void Draw(const ViewProjection& viewProjection);

private:
	WorldTransform worldTransForm_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Phase phase_ = Phase::Approach;
};
