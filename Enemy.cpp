#include "Enemy.h"

Enemy::Enemy(){
	worldTransForm.Initialize();
	isDead = false;
	YTmp = { 0,1,0 };
}

Enemy::~Enemy(){}

void Enemy::CalcVec(Vector3 obj) {
		//���ʉ��x�N�g��
		enemyTmp = obj - worldTransForm.translation_;
		enemyTmp.normalize();
		//�E�x�N�g��
		enemyRight = YTmp.cross(enemyTmp);
		enemyRight.normalize();
		//���ʃx�N�g��
		enemyFront = enemyRight.cross(YTmp);
		enemyFront.normalize();
}

void Enemy::Update(Vector3 obj) {
	//�x�N�g���v�Z
	CalcVec(obj);

	//�s��v�Z
	worldTransForm.matWorld_ = Affin::matUnit();
	worldTransForm.matWorld_ = Affin::matWorld(
		worldTransForm.translation_,
		worldTransForm.rotation_,
		worldTransForm.scale_);

		if (isDead == false) {
			worldTransForm.translation_ += enemyFront* 0.008;
		}

		//���ʂ𔽉f
		worldTransForm.TransferMatrix();
}