#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"

#include "Bullet.h"

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

	void Attack();

	float Angle(float angle);

	void Reticle3D();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	/// 
	/// 
	uint32_t textureHandle_[10];

	Sprite* sprite_ = nullptr;
	Model* model_ = nullptr;
	WorldTransform objHome_;
	WorldTransform floor_;
	WorldTransform worldTransforms_[10];
	ViewProjection viewProjection_;

	DebugCamera* debugCamera_ = nullptr;

	//カメラ上方向の角度
	float viewAngle = 0.0f;

	//弾
	Bullet* bullet_ = nullptr;
	Vector3 pos;
	Vector3 moveBul;
	Vector3 velo;
	Vector3 ai;

	float KEyeSpeed = 0.1f;


	// 3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;
	float kDistancePlayerTo3DReticle = 15.0f;
	Vector3 ret3DPos;
	Vector3 myPos;
	Vector3 resultRet;
};
