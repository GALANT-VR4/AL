#include "GameScene.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete enemy_;
	delete skydome_;
	delete modelSkydome_;
	delete railCamera_;
	delete fog_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("sample.png");
	model_ = Model::Create();

	fog_ = Model::CreateFromOBJ("fog", true);

	viewProjection_.Initialize();

	railCamera_ = new RailCamera();
	railCamera_->Initialize({0.f, 0.f, -20.f}, {0.f, 0.f, 0.f});

	player_ = new Player();
	Vector3 playerPosition(0, -5.0f, 0.0f);
	player_->Initialize(model_, textureHandle_, playerPosition);
	player_->SetParent(&railCamera_->GetWorldTransform());

	enemy_ = new Enemy();
	enemy_->Initialize(model_);

	enemy_->SetPlayer(player_);

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);

	fogWorldTransform_.Initialize();

	for (int i = 0; i < 10; i++) {
		sprintf_s(numTexDir_, 8, "%d.png", i);
		numTex_[i] = TextureManager::Load(numTexDir_);
		num_[i] = Sprite::Create(numTex_[i], {0, 0});
		hpNum_[0][i] = Sprite::Create(numTex_[i], {128, 0});
		hpNum_[1][i] = Sprite::Create(numTex_[i], {160, 0});
		for (int j = 0; j < 3; j++) {
			scoreNum_[j][i] = Sprite::Create(numTex_[i], {(float)128 + j * 32, 48});
			resultScoreNum_[j][i] = Sprite::Create(numTex_[i], {(float)468 + 128 + j * 32, 448});
		}
		for (int j = 0; j < 3; j++) {
			bestNum_[j][i] = Sprite::Create(numTex_[i], {(float)128 + j * 32, 96});
			resultBestNum_[j][i] = Sprite::Create(numTex_[i], {(float)468 + 128 + j * 32, 496});
		}
	}

	titleTex_ = TextureManager::Load("title.png");
	title_ = Sprite::Create(titleTex_, {0, 0});

	resultTex_ = TextureManager::Load("result.png");
	result_ = Sprite::Create(resultTex_, {0, 0});

	hpTex_ = TextureManager::Load("hp.png");
	hp_ = Sprite::Create(hpTex_, {0, 0});

	scoreTex_ = TextureManager::Load("score.png");
	score_ = Sprite::Create(scoreTex_, {0, 48});
	resultScore_ = Sprite::Create(scoreTex_, {468, 448});

	bestTex_ = TextureManager::Load("best.png");
	best_ = Sprite::Create(bestTex_, {0, 96});
	resultBest_ = Sprite::Create(bestTex_, {468, 496});

	pushSpaceTex_ = TextureManager::Load("pushSpace.png");
	pushSpace_ = Sprite::Create(pushSpaceTex_, {488, 600});

	attackAudio_ = audio_->LoadWave("attack.wav");
	hitAudio_ = audio_->LoadWave("hit.wav");
	resultAudio_ = audio_->LoadWave("result.wav");
	player_->SetAudio(audio_, attackAudio_, hitAudio_);
}

void GameScene::Update() {
	switch (scene_) {
	case menu:
		timer_--;
		if (input_->PushKey(DIK_SPACE))
			timer_ = 45;
		if (timer_ == 0)
			scene_ = play;
		break;
	case play:
		player_->Update();
		enemy_->Update();
		CheckAllCollisions();
		skydome_->Update();
		railCamera_->Update();
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
		fogWorldTransform_.TransferMatrix();
		fogWorldTransform_.UpdateMatrix();
		if (player_->GetHp() <= 0) {
			audio_->PlayWave(resultAudio_);
			scene_ = result;
			timer_ = 45;
		}
		break;
	case result:
		timer_--;
		if (player_->GetHp() == 0 && timer_ < 0 && input_->PushKey(DIK_SPACE)) {
			timer_ = 45;
			player_->SetHp();
			if (player_->GetScore() > bestScore_)
				bestScore_ = player_->GetScore();
			player_->ResetScore();
		}
		if (player_->GetHp() != 0 && timer_ < 0)
			scene_ = play;
		break;
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	switch (scene_) {
	case menu:
#pragma region 背景スプライト描画
		// 背景スプライト描画前処理
		Sprite::PreDraw(commandList);

		/// <summary>
		/// ここに背景スプライトの描画処理を追加できる
		/// </summary>
		if (timer_ < 0)
			title_->Draw();
		// スプライト描画後処理
		Sprite::PostDraw();
		// 深度バッファクリア
		dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
		// 3Dオブジェクト描画前処理
		Model::PreDraw(commandList);

		/// <summary>
		/// ここに3Dオブジェクトの描画処理を追加できる
		/// </summary>
		if (timer_ <= 60 && timer_ > 0)
			skydome_->Draw(viewProjection_);

		// 3Dオブジェクト描画後処理
		Model::PostDraw();
#pragma endregion
		break;
	case play:
#pragma region 3Dオブジェクト描画
		// 3Dオブジェクト描画前処理
		Model::PreDraw(commandList);

		/// <summary>
		/// ここに3Dオブジェクトの描画処理を追加できる
		/// </summary>
		player_->Draw(viewProjection_);
		enemy_->Draw(viewProjection_);
		skydome_->Draw(viewProjection_);

		// 3Dオブジェクト描画後処理
		Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
		// 前景スプライト描画前処理
		Sprite::PreDraw(commandList);
		/// <summary>
		/// ここに前景スプライトの描画処理を追加できる
		/// </summary>
		hp_->Draw();
		hpNum_[0][int(player_->GetHp() / 10)]->Draw();
		hpNum_[1][player_->GetHp() % 10]->Draw();
		score_->Draw();
		scoreNum_[0][int(player_->GetScore() / 100)]->Draw();
		scoreNum_[1][int(int(player_->GetScore() % 100) / 10)]->Draw();
		scoreNum_[2][player_->GetScore() % 10]->Draw();
		best_->Draw();
		bestNum_[0][int(bestScore_ / 100)]->Draw();
		bestNum_[1][int(int(bestScore_ % 100) / 10)]->Draw();
		bestNum_[2][bestScore_ % 10]->Draw();

		// スプライト描画後処理
		Sprite::PostDraw();

#pragma endregion

		break;
	case result:
#pragma region 背景スプライト描画
		// 背景スプライト描画前処理
		Sprite::PreDraw(commandList);

		/// <summary>
		/// ここに背景スプライトの描画処理を追加できる
		/// </summary>
		result_->Draw();
		if (player_->GetHp() == 0 && timer_ < 0)
			pushSpace_->Draw();
		resultScore_->Draw();
		resultBest_->Draw();
		resultScoreNum_[0][int(player_->GetScore() / 100)]->Draw();
		resultScoreNum_[1][int(int(player_->GetScore() % 100) / 10)]->Draw();
		resultScoreNum_[2][player_->GetScore() % 10]->Draw();
		resultBestNum_[0][int(bestScore_ / 100)]->Draw();
		resultBestNum_[1][int(int(bestScore_ % 100) / 10)]->Draw();
		resultBestNum_[2][bestScore_ % 10]->Draw();

		// スプライト描画後処理
		Sprite::PostDraw();
		// 深度バッファクリア
		dxCommon_->ClearDepthBuffer();
#pragma endregion

		break;
	}
}

void GameScene::CheckAllCollisions() {
	Vector3 posA, posB;
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	posA = player_->GetWorldPosition();
	for (EnemyBullet* bullet : enemyBullets) {
		posB = bullet->GetWorldPosition();
		if (pow(posB.x - posA.x, 2) + pow(posB.y - posA.y, 2) + pow(posB.z - posA.z, 2) <= 4) {
			player_->OnCollision();
			bullet->OnCollision();
		}
	}

	posA = enemy_->GetWorldposition();
	for (PlayerBullet* bullet : playerBullets) {
		posB = bullet->GetWorldPosition();
		if (pow(posB.x - posA.x, 2) + pow(posB.y - posA.y, 2) + pow(posB.z - posA.z, 2) <= 4) {
			enemy_->onCollision();
			bullet->OnCollision();
		}
	}

	for (PlayerBullet* bulletA : playerBullets) {
		posA = bulletA->GetWorldPosition();
		for (EnemyBullet* bulletB : enemyBullets) {
			posB = bulletB->GetWorldPosition();
			if (pow(posB.x - posA.x, 2) + pow(posB.y - posA.y, 2) + pow(posB.z - posA.z, 2) <= 4) {
				bulletA->OnCollision();
				player_->scoreAdd();
				bulletB->OnCollision();
			}
		}
	}
}
