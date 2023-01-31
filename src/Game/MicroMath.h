#pragma once

#include <math.h>

class Point2
{
public:
	int x = 0;
	int y = 0;
};

class Color
{
public:
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	unsigned char a = 0;
};

class Vector2
{
public:
	float x = 0.0f;
	float y = 0.0f;
};

class Vector3
{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

class Matrix4
{
public:
	float m[16];
};

