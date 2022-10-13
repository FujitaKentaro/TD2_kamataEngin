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
	//右ベクトル
	Vector3 enemyRight;
	//左ベクトル
	Vector3 enemyLeft;
	//正面ベクトル
	Vector3 enemyFront;
	//加速
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