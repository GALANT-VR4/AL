#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "Enemy.h"
#include "Player.h"
#include "RailCamera.h"
#include "Skydome.h"

typedef enum {
	menu,
	play,
	result,
} Scene;

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void CheckAllCollisions();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	uint32_t textureHandle_ = 0;
	Model* model_ = nullptr;

	Model* modelSkydome_ = nullptr;

	Model* fog_ = nullptr;

	ViewProjection viewProjection_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
	Skydome* skydome_ = nullptr;
	RailCamera* railCamera_;
	uint32_t numTex_[10] = {0};
	char numTexDir_[8];
	Sprite* num_[10];

	uint32_t attackAudio_;
	uint32_t hitAudio_;
	uint32_t resultAudio_;

	Sprite* hpNum_[2][10];
	Sprite* scoreNum_[3][10];
	Sprite* bestNum_[3][10];
	Sprite* resultScore_;
	Sprite* resultScoreNum_[3][10];
	Sprite* resultBest_;
	Sprite* resultBestNum_[3][10];

	uint32_t titleTex_;
	Sprite* title_ = nullptr;

	uint32_t resultTex_;
	Sprite* result_ = nullptr;

	uint32_t hpTex_;
	Sprite* hp_ = nullptr;

	uint32_t scoreTex_;
	Sprite* score_ = nullptr;

	uint32_t bestTex_;
	Sprite* best_;

	uint32_t pushSpaceTex_;
	Sprite* pushSpace_ = nullptr;

	WorldTransform fogWorldTransform_;

	Vector3 fogpos = {.0f, .0f, .0f};

	Scene scene_ = result;

	int timer_ = 0;

	int bestScore_ = 0;
};
