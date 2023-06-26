#pragma once
#include "stdafx.h"


struct Vector2 {
	FLOAT x, y;

	Vector2() {
		x = y = 0.0f;
	}

	Vector2(FLOAT x, FLOAT y) {
		this->x = x;
		this->y = y;
	}

	FLOAT distance(Vector2 &Vector) {
		return sqrt(distanceEx(Vector));
	}

	FLOAT distanceEx(Vector2 &Vector) {
		FLOAT _x = this->x - Vector.x, _y = this->y - Vector.y;
		return ((_x * _x) + (_y * _y));
	}
};

struct Vector3 {
	FLOAT x, y, z;
	
	Vector3() {
		x = y = z = 0.0f;
	}

	Vector3(FLOAT x, FLOAT y, FLOAT z) {
		this->x = x; this->y = y; this->z = z;
	}

	BOOL operator==(Vector3 &Vector) {
		return (this->x == Vector.x && this->y == Vector.y && this->z == Vector.z);
	}
	
	Vector3& operator+(Vector3 &Vector) {
		return Vector3(this->x + Vector.x, this->y + Vector.y, this->z + Vector.z);
	}
	
	Vector3& operator-(Vector3 &Vector) {
		return Vector3(this->x - Vector.x, this->y - Vector.y, this->z - Vector.z);
	}

	FLOAT distance(Vector3 &Vector) {
		return sqrt(distanceEx(Vector));
	}

	FLOAT distanceEx(Vector3 &Vector) {
		FLOAT _x = this->x - Vector.x, _y = this->y - Vector.y, _z = this->z - Vector.z;
		return ((_x * _x) + (_y * _y) + (_z * _z));
	}

	FLOAT dotProduct(Vector3 &Vector) {
		return (this->x * Vector.x) + (this->y * Vector.y) + (this->z * Vector.z);
	}
};

struct Vector4 {
	FLOAT a, b, c, d;

	Vector4() {
		 a = b = c = d = 0.0f;
	}

	Vector4(FLOAT a, FLOAT b, FLOAT c, FLOAT d)	{
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = d;
	}
};