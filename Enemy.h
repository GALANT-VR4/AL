#pragma once
#include "EnemyBullet.h"
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
	void Fire();
	~Enemy();
	void AppInit();

private:
	WorldTransform worldTransForm_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Phase phase_ = Phase::Approach;
	EnemyBullet* bullet_ = nullptr;
	std::list<EnemyBullet*> bullets_;
	static const int kFireInterval = 60;
	int32_t fireTimer = 0;
};
