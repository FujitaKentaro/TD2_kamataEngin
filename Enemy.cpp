#include "Enemy.h"

Enemy::Enemy(){
	worldTransForm.Initialize();
	isDead = false;
	YTmp = { 0,1,0 };
	speed = 0.0008f;
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
			time++;
			if (time == 2) {
				speed += 0.0001f;
				time = 0;
			}
			worldTransForm.translation_ += enemyFront* speed;
		}

		//���ʂ𔽉f
		worldTransForm.TransferMatrix();
}