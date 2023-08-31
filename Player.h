#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "PlayerBullet.h"
#include "GameScene.h"
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
	void SetParent(const WorldTransform* parent);
	void scoreAdd() { score_++; }
	int GetHp() { return hp_; }
	void SetHp(int hp = 25) { hp_ = hp; }
	int GetScore() { return score_; }
	void ResetScore() { score_ = 0; }
	void SetAudio(Audio* audio, uint32_t audioHandle1, uint32_t audioHandle2) {
		audio_ = audio;
		attackAudioHandle_ = audioHandle1;
		hitAudiohandle_ = audioHandle2;
	}

private:
	WorldTransform worldTransForm_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	PlayerBullet* bullet_ = nullptr;
	std::list<PlayerBullet*>bullets_;
	int fireTimer_ = 0;
	int hp_ = 25;
	int score_ = 0;
	Audio* audio_ = nullptr;
	uint32_t attackAudioHandle_;
	uint32_t hitAudiohandle_;
};
