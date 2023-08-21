#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "PlayerBullet.h"
#include <list>
class Player {
public:
	void Initialize(Model* model, uint32_t textureHandle, Vector3 playerPosition);
	void Update();
	void Draw(ViewProjection& viewProjection);
	void Rotate();
	void Attack();
	~Player();
	Vector3 GetWorldPosition();
	void OnCollision();
	const std::list<PlayerBullet* > &GetBullets() const { return bullets_; }

private:
	WorldTransform worldTransForm_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	PlayerBullet* bullet_ = nullptr;
	std::list<PlayerBullet*>bullets_;
};
