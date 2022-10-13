#pragma once

#include "WorldTransform.h"
#include "Bullet.h"
#include <random>

class Enemy {
public:
	WorldTransform worldTransForm;
	int isDead;
	Vector3 enemyTmp;
	Vector3 YTmp;
	//�E�x�N�g��
	Vector3 enemyRight;
	//���x�N�g��
	Vector3 enemyLeft;
	//���ʃx�N�g��
	Vector3 enemyFront;
	//����
	float speed;
	//time
	int time;

public:
	Enemy();
	~Enemy();
	void CalcVec(Vector3 view);
	void Update(Vector3 obj);
	void Pop();
	void Hit();
};