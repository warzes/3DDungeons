#pragma once

FlyingCamera cam;

void ExampleInit()
{
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

	DebugDraw::Flush(view * perpective); // TODO: ��������� �������

	puts(std::to_string(Angle(t1, t2)).c_str());



}