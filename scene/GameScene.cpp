#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

#include "PrimitiveDrawer.h"

#include <random>
#include"Affin.h"
#define PI 3.14

float GameScene::Angle(float angle)
{
	return angle * PI / 180;
}



float Clamp(float min, float max, float num) {
	if (min > num) {
		return min;
	}
	else if (max < num) {
		return max;
	}
	return num;
}
GameScene::GameScene() {}

GameScene::~GameScene() {
	delete sprite_;
	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを入れ込む
	textureHandle_[0] = TextureManager::Load("mario.jpg");
	textureHandle_[1] = TextureManager::Load("dog.png");
	textureHandle_[2] = TextureManager::Load("png.png");

	//スプライトの生成
	sprite_ = Sprite::Create(textureHandle_[0], { 100,50 });
	//3Dモデルの生成
	model_ = Model::Create();
	//ワールドトランスフォームの初期化
	// 中心OBJ
	objHome_.Initialize();
	objHome_.scale_ = { 3,3,3 };
	objHome_.translation_ = { 0,1,0 };
	// 床OBJ
	floor_.Initialize();
	floor_.translation_ = { 0,-1,0 };
	floor_.scale_ = { 150,0.1f,150 };

	//ワールドトランスフォームの初期化
	worldTransforms_[0].Initialize();
	worldTransforms_[0].scale_ = { 3,3,3 };
	worldTransforms_[0].translation_ = { 0,1,0 };

	worldTransforms_[1].Initialize();
	worldTransforms_[1].translation_ = { 0,15,15 };
	worldTransforms_[1].parent_ = &worldTransforms_[0];

	for (int i = 2; i < 10; i++) {
		worldTransforms_[i].Initialize();
	}

	worldTransform3DReticle_.Initialize();

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	viewProjection_.target = Affin::GetWorldTrans(worldTransforms_[0].matWorld_);
	viewProjection_.eye = Affin::GetWorldTrans(worldTransforms_[1].matWorld_);
	viewProjection_.UpdateMatrix();


	enemys[0].worldTransForm.translation_ = { -10,0,0 };
	enemys[1].worldTransForm.translation_ = { 10,0,0 };
	enemys[2].worldTransForm.translation_ = { 0,0,-10 };
	enemys[3].worldTransForm.translation_ = { 0,0,10 };
	enemys[4].worldTransForm.translation_ = { 15,0,10 };
}

void GameScene::Update() {

	ai = Affin::GetWorldTrans(worldTransforms_[1].matWorld_);
	viewProjection_.eye = { ai.x,ai.y,ai.z };
	viewProjection_.UpdateMatrix();

	//yの仮ベクトル
	Vector3 yTmpVec(0, 1, 0);
	yTmpVec.normalize();
	//正面仮ベクトル
	Vector3 frontTmp = viewProjection_.target - viewProjection_.eye;
	frontTmp.normalize();
	//右ベクトル
	Vector3 rightVec = yTmpVec.cross(frontTmp);
	rightVec.normalize();
	//左ベクトル
	Vector3 leftVec = frontTmp.cross(yTmpVec);
	leftVec.normalize();
	//正面ベクトル
	Vector3 frontVec = rightVec.cross(yTmpVec);
	frontVec.normalize();
	//背面ベクトル
	Vector3 behindVec = frontVec * -1;

	//視点の移動速さ

	int a;
	float kCharacterSpeed = 0.1f;

	Vector3 move = { 0,0,0 };

	{	// 中心オブジェクト
		objHome_.matWorld_ = Affin::matUnit();
		objHome_.matWorld_ = Affin::matWorld(objHome_.translation_, objHome_.rotation_, objHome_.scale_);
		objHome_.TransferMatrix();
	}

	{
		// 回転処理
		if (input_->PushKey(DIK_RIGHT)) {

			if (KEyeSpeed > 0.0f) {
				KEyeSpeed *= -1;
			}
		}
		else if (input_->PushKey(DIK_LEFT)) {
			if (KEyeSpeed < 0.0f) {
				KEyeSpeed *= -1;
			}
		}
		// 親オブジェクト
		worldTransforms_[0].rotation_.y += KEyeSpeed;
	}
	

	for (int i = 0; i < _countof(worldTransforms_); i++) {

		worldTransforms_[i].matWorld_ = Affin::matUnit();
		worldTransforms_[i].matWorld_ = Affin::matWorld(
			worldTransforms_[i].translation_,
			worldTransforms_[i].rotation_,
			worldTransforms_[i].scale_);

		if (worldTransforms_[i].parent_ != nullptr) {
			worldTransforms_[i].matWorld_ *= worldTransforms_[i].parent_->matWorld_;
		}

		worldTransforms_[i].TransferMatrix();

	}
	//// 子の更新(子は子で初期化)
	//{	// カメラ用
	//	worldTransforms_[1].matWorld_ = Affin::matUnit();
	//	worldTransforms_[1].matWorld_ = Affin::matTrans(worldTransforms_[1].translation_);
	//	worldTransforms_[1].matWorld_ *= worldTransforms_[1].parent_->matWorld_;
	//	worldTransforms_[1].TransferMatrix();
	//}
	//{
	//	worldTransforms_[2].matWorld_ = Affin::matUnit();
	//	worldTransforms_[2].matWorld_ = Affin::matWorld(worldTransforms_[2].translation_, worldTransforms_[2].rotation_, worldTransforms_[2].scale_);
	//	//3つ目
	//	//worldTransforms_[2].matWorld_ *= worldTransforms_[2].parent_->matWorld_;
	//	worldTransforms_[2].TransferMatrix();
	//}
	//{
	//	worldTransforms_[3].matWorld_ = Affin::matUnit();
	//	worldTransforms_[3].matWorld_ = Affin::matWorld(worldTransforms_[3].translation_, worldTransforms_[3].rotation_, worldTransforms_[3].scale_);
	//	//4つ目
	//	//worldTransforms_[2].matWorld_ *= worldTransforms_[2].parent_->matWorld_;
	//	worldTransforms_[3].TransferMatrix();
	//}
	//{
	//	worldTransforms_[4].matWorld_ = Affin::matUnit();
	//	worldTransforms_[4].matWorld_ = Affin::matTrans(worldTransforms_[4].translation_);
	//	//5つ目
	//	worldTransforms_[4].matWorld_ *= worldTransforms_[4].parent_->matWorld_;
	//	worldTransforms_[4].TransferMatrix();
	//}

	{	// 床
		floor_.matWorld_ = Affin::matUnit();
		floor_.matWorld_ = Affin::matWorld(floor_.translation_, floor_.rotation_, floor_.scale_);
		floor_.TransferMatrix();
	}

	//自機のワールド座標から3Dレティクルのワールド座標を計算
	//自機から3Dレティクルへの距離	

	if (input_->PushKey(DIK_DOWN)) {
		kDistancePlayerTo3DReticle = 20;

	}
	else if (input_->PushKey(DIK_UP)) {
		kDistancePlayerTo3DReticle = 10;

	}
	else {
		kDistancePlayerTo3DReticle = 15;
	}

	Reticle3D();

	Attack();

	if (bullet_)
	{
		bullet_->Update(resultRet);
	}

	//敵更新
	for (int i = 0; i < _countof(enemys); i++) {
		enemys[i].Update(objHome_.translation_);
	}

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

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
	model_->Draw(objHome_, viewProjection_, textureHandle_[2]);
	//model_->Draw(worldTransforms_[0], viewProjection_, textureHandle_[0]);
	model_->Draw(worldTransforms_[1], viewProjection_, textureHandle_[0]);
	/*model_->Draw(worldTransforms_[2], viewProjection_, textureHandle_[0]);
	model_->Draw(worldTransforms_[3], viewProjection_, textureHandle_[0]);
	model_->Draw(worldTransforms_[4], viewProjection_, textureHandle_[0]);*/
	model_->Draw(floor_, viewProjection_, textureHandle_[1]);

	model_->Draw(worldTransform3DReticle_, viewProjection_, textureHandle_[0]);
	for (int i = 0; i < _countof(enemys); i++) {
		model_->Draw(enemys[i].worldTransForm, viewProjection_, textureHandle_[0]);
	}

	if (bullet_) {
		bullet_->Draw(viewProjection_);
	}
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::Attack()
{
	if (input_->PushKey(DIK_SPACE))
	{
		//弾を生成し、初期化
		Bullet* newbullet = new Bullet();
		pos = Affin::GetWorldTrans(worldTransforms_[1].matWorld_);
		ret3DPos = Affin::GetWorldTrans(worldTransform3DReticle_.matWorld_);
		myPos = Affin::GetWorldTrans(worldTransforms_[1].matWorld_);
		velo = ret3DPos - myPos;
		velo.normalize();
		resultRet = velo * newbullet->speed;
		newbullet->Initialize(model_, pos);


		//弾を登録
		bullet_ = newbullet;
	}
}

void GameScene::Reticle3D() {
	//自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset = { 0.0f, 0, 1.0f };
	//自機のワールド行列の回転を反映
	offset = Affin::VecMat(offset, worldTransforms_[1].matWorld_);
	//ベクトルの長さを整える
	//offset.normalize();
	float len = sqrt(offset.x * offset.x + offset.y * offset.y + offset.z * offset.z);
	if (len != 0) {
		offset /= len;
	}
	offset *= kDistancePlayerTo3DReticle;
	worldTransform3DReticle_.translation_ = offset;
	worldTransform3DReticle_.matWorld_ = Affin::matTrans(worldTransform3DReticle_.translation_);
	worldTransform3DReticle_.TransferMatrix();

	DebugText::GetInstance()->SetPos(20, 260);
	DebugText::GetInstance()->Printf(
		"ReticleObject:(%f,%f,%f)", worldTransform3DReticle_.translation_.x,
		worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);

}
