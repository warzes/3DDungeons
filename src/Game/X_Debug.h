#pragma once

#include <DirectXMath.h>

FlyingCamera cam;

class Matrix4x4f
{
public:
	float m_Data[16];

	Matrix4x4f& SetIdentity();
	void SetPosition(const Vector3& v);

	float& Get(int row, int column) { return m_Data[row + (column * 4)]; }
	const float& Get(int row, int column)const { return m_Data[row + (column * 4)]; }

};

Matrix4x4f& Matrix4x4f::SetIdentity()
{
	Get(0, 0) = 1.0;	Get(0, 1) = 0.0;	Get(0, 2) = 0.0;	Get(0, 3) = 0.0;
	Get(1, 0) = 0.0;	Get(1, 1) = 1.0;	Get(1, 2) = 0.0;	Get(1, 3) = 0.0;
	Get(2, 0) = 0.0;	Get(2, 1) = 0.0;	Get(2, 2) = 1.0;	Get(2, 3) = 0.0;
	Get(3, 0) = 0.0;	Get(3, 1) = 0.0;	Get(3, 2) = 0.0;	Get(3, 3) = 1.0;
	return *this;
}

inline void Matrix4x4f::SetPosition(const Vector3& v)
{
	Get(0, 3) = v.x; Get(1, 3) = v.y; Get(2, 3) = v.z;
}

typedef struct Matrix {
	float m0, m4, m8, m12;  // Matrix first row (4 components)
	float m1, m5, m9, m13;  // Matrix second row (4 components)
	float m2, m6, m10, m14; // Matrix third row (4 components)
	float m3, m7, m11, m15; // Matrix fourth row (4 components)
} Matrix;

Matrix MatrixTranslate(float x, float y, float z)
{
	Matrix result = { 1.0f, 0.0f, 0.0f, x,
					  0.0f, 1.0f, 0.0f, y,
					  0.0f, 0.0f, 1.0f, z,
					  0.0f, 0.0f, 0.0f, 1.0f };

	return result;
}


struct matrix4
{
	Vector4 a, b, c, d;

	void settranslation(float x, float y, float z) { d.x = x; d.y = y; d.z = z; }
};

void ExampleInit()
{
	Matrix4 t1;
	t1.SetTranslation({ 2.0f, 3.0f, 4.0f }); // мой
	/*
	1 0 0 t
	0 1 0 t
	0 0 1 t
	0 0 0 1
	*/

	Matrix4x4f t2;
	t2 = t2.SetIdentity();
	t2.SetPosition({ 2.0f, 3.0f, 4.0f }); // юнити
	/*
1 0 0 0
0 1 0 0
0 0 1 0
t t t 1
*/

	DirectX::XMMATRIX t3 = DirectX::XMMatrixTranslation(2.0f, 3.0f, 4.0f);
	/*
1 0 0 0
0 1 0 0
0 0 1 0
t t t 1
*/
	Matrix t4 = MatrixTranslate(2.0f, 3.0f, 4.0f); // рейлиб
	float matMVPfloat[16] = {
				t4.m0, t4.m1, t4.m2, t4.m3,
				t4.m4, t4.m5, t4.m6, t4.m7,
				t4.m8, t4.m9, t4.m10, t4.m11,
				t4.m12, t4.m13, t4.m14, t4.m15
	};
	/*
	1 0 0 0
	0 1 0 0
	0 0 1 0
	t t t 1
	*/

	matrix4 t5;
	t5.settranslation(2.0f, 3.0f, 4.0f); // куб
	/*
1 0 0 0
0 1 0 0
0 0 1 0
t t t 1
*/











	Matrix3x4 m1(
		1.1f, 2.2f, 3.3f, 4.4f,
		5.5f, 6.6f, 7.7f, 8.8f,
		9.9f, 10.0f, 11.0f, 12.0f);

	Matrix3 m3_1(
		1.1f, 2.2f, 3.3f,
		4.4f, 5.5f, 6.6f,
		7.7f, 8.8f, 9.9f);

	Matrix4 m2(
		4.1f, 1.2f, 1.3f, 3.4f,
		7.5f, 2.6f, 4.7f, 5.8f,
		2.9f, 3.4f, 5.6f, 2.8f,
		2.1f, 5.2f, 7.8f, 9.9f);


	Matrix4 m10 = m2.Inverse();

	cam.Look({ 0.0f, 3.0f, 6.0f }, { 0.0f, 0.0f, 0.0f });
}

void ExampleClose()
{
}

void ExampleFrame()
{
	auto deltaCursor = GetCursorDelta();
	if( deltaCursor.x != 0 || deltaCursor.y != 0 )
	{
		if( IsMouseButtonDown(MouseButton::Right) )
			cam.OnMouseMove(deltaCursor.x, deltaCursor.y);
	}

	short Keys = 0x0000;
	if( IsKeyDown('W') ) Keys |= CAMERA_KEY_W;
	if( IsKeyDown('S') ) Keys |= CAMERA_KEY_S;
	if( IsKeyDown('A') ) Keys |= CAMERA_KEY_A;
	if( IsKeyDown('D') ) Keys |= CAMERA_KEY_D;
	if( IsKeyDown('R') ) Keys |= CAMERA_KEY_R;
	if( IsKeyDown('F') ) Keys |= CAMERA_KEY_F;
	if( IsKeyDown('Q') ) Keys |= CAMERA_KEY_Q;
	if( IsKeyDown('E') ) Keys |= CAMERA_KEY_E;
	if( IsKeyDown('C') ) Keys |= CAMERA_KEY_E;

	if( IsKeyDown(0x20/*VK_SPACE*/) ) Keys |= CAMERA_KEY_SPACE;
	if( IsKeyDown(0x10/*VK_SHIFT*/) ) Keys |= CAMERA_KEY_SHIFT;
	if( IsKeyDown(0x11/*VK_CONTROL*/) ) Keys |= CAMERA_KEY_CONTROL;

	Vector3 Movement;
	bool MoveCamera = cam.OnKeys(Keys, 0.01f, Movement);
	if( MoveCamera ) cam.Move(Movement);

	Matrix4 view = cam.GetViewMatrix();
	Matrix4 perpective = Matrix4::Perspective(45.0f * DEG2RAD, GetWindowAspectRatio(), 0.01f, 1000.f);

	Vector3 t1 = { -10.0f, 0.0f, 0.0f };
	Vector3 t2 = { -5.0f, 5.0f, 0.0f };

	DebugDraw::DrawLine({ 0.0f, 0.0f, 0.0f }, t1, RED);
	DebugDraw::DrawLine({ 0.0f, 0.0f, 0.0f }, t2, GREEN);

	//DebugDraw::DrawGrid(10.0f);

	DebugDraw::Flush(view * perpective); // TODO: пофиксить порядок

	puts(std::to_string(Angle(t1, t2)).c_str());



}