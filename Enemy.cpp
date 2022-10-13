#include "Enemy.h"

Enemy::Enemy(){
	worldTransForm.Initialize();
	isDead = false;
	YTmp = { 0,1,0 };
	speed = 0.0008f;
}

Enemy::~Enemy(){}

void Enemy::CalcVec(Vector3 obj) {
		//正面仮ベクトル
		enemyTmp = obj - worldTransForm.translation_;
		enemyTmp.normalize();
		//右ベクトル
		enemyRight = YTmp.cross(enemyTmp);
		enemyRight.normalize();
		//正面ベクトル
		enemyFront = enemyRight.cross(YTmp);
		enemyFront.normalize();
}

void Enemy::Update(Vector3 obj) {
	//ベクトル計算
	CalcVec(obj);

	//行列計算
	worldTransForm.matWorld_ = Affin::matUnit();
	worldTransForm.matWorld_ = Affin::matWorld(
		worldTransForm.translation_,
		worldTransForm.rotation_,
		worldTransForm.scale_);

		if (isDead == false) {
			time++;
			if (time == 2) {
				speed += 0.0001f;
				time = 0;
			}
			worldTransForm.translation_ += enemyFront* speed;
		}

		//結果を反映
		worldTransForm.TransferMatrix();
}