#include "PlayerCamera.h"

FlyingCamera camera;
bool IsFreeCameraRotate = false;
bool IsFreeCameraMov = false;

Vector3 playerPos;
Vector3 playerEyeTarget;

// turn camera
bool m_isMoving = false;
bool m_isTurning = false;
bool m_continuedWalk = true;
float m_walkSpeed = 2.0f;
float m_stepSize = 1.0f;
Vector3 m_targetPosition = { 0.0f, 0.0f, 0.0f };
Vector3 m_direction = { 0 };

float TargetRotateAngle = 0.0f;
float TargetRotateAngle2 = 0.0f;

enum class moveDir
{
	No,
	Forward,
	Back,
	Left,
	Right
};

enum class rotateDirY
{
	Left,
	No,
	Right
};

rotateDirY m_rotateDir = rotateDirY::No;
float m_currentRotateY = 0.0f;
float m_endRotateY = 0.0f;
float m_speedRotate = 200.0f;

void move(moveDir dir)
{
	Vector3 Up = camera.y;
	Vector3 Right = camera.x;
	Vector3 Forward = camera.z;

	if( dir == moveDir::Forward ) m_direction = Forward;
	if( dir == moveDir::Back ) m_direction = -Forward;
	if( dir == moveDir::Left ) m_direction = -Right;
	if( dir == moveDir::Right ) m_direction = Right;

	//const Vector3 targetPos = Vector3Add(m_cameraPosition, m_direction);
	//if( !isBlocked(targetPos, map) && dir != moveDir::No )
	{
		m_isMoving = true;
		m_targetPosition = camera.position + m_direction;
	}
}


float angleRotateCamera = 1.0f;

void turn(rotateDirY dir)
{
	if( !m_isTurning && dir != rotateDirY::No && TargetRotateAngle2 == 0.0f )
	{
		m_rotateDir = dir;
		m_isTurning = true;

		if( dir == rotateDirY::Left )
		{
			angleRotateCamera = -1.0f;
			TargetRotateAngle2 = -90.0f * DEG2RAD;
		}
		if( dir == rotateDirY::Right )
		{
			angleRotateCamera = 1.0f;
			TargetRotateAngle2 = 90.0f * DEG2RAD;
		}




		if( dir == rotateDirY::Left && m_currentRotateY <= 0.0f ) m_currentRotateY = 360.0f;
		else if( dir == rotateDirY::Right && m_currentRotateY >= 360.0f ) m_currentRotateY = 0.0f;

		if( dir == rotateDirY::Left )
			m_endRotateY = m_currentRotateY - 90.0f;
		else if( dir == rotateDirY::Right )
			m_endRotateY = m_currentRotateY + 90.0f;
	}
}

void PlayerCamera::SetPosition(const Vector3& pos, const Vector3& eyeTarget)
{
	playerPos = pos;
	m_targetPosition = pos;
	playerEyeTarget = eyeTarget;
	camera.Look(pos, eyeTarget);
}

void PlayerCamera::Update(bool FreeCameraRotate, bool FreeCameraMove)
{
	if( FreeCameraRotate )
	{
		IsFreeCameraRotate = FreeCameraRotate;
		auto deltaCursor = GetCursorDelta();
		if( deltaCursor.x != 0 || deltaCursor.y != 0 )
		{
			if( IsMouseButtonDown(MouseButton::Right) )
				camera.OnMouseMove(deltaCursor.x, deltaCursor.y);
		}
	}
	else
	{
		if( IsFreeCameraRotate )
		{
			// TODO: вернуть камеру обратно
			IsFreeCameraRotate = false;
		}
		else
		{

		}
	}

	if( FreeCameraMove )
	{
		IsFreeCameraMov = FreeCameraMove;
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
		bool MoveCamera = camera.OnKeys(Keys, GetDeltaTime(), Movement);
		if( MoveCamera ) camera.Move(Movement);
	}
	else
	{
		if( !m_isTurning )
		{
			if( !m_isMoving ) // во время вращения камеры нельзя двигаться, поэтому условие вложенное
			{
				if( IsKeyDown('W') ) move(moveDir::Forward);
				if( IsKeyDown('S') ) move(moveDir::Back);
				if( IsKeyDown('Q') ) move(moveDir::Left);
				if( IsKeyDown('E') ) move(moveDir::Right);
			}

			if( IsKeyPressed('A') ) turn(rotateDirY::Left);
			if( IsKeyPressed('D') ) turn(rotateDirY::Right);
		}

		Vector3 capPos = camera.position;

		if( m_isMoving )
		{
			capPos += m_walkSpeed * m_direction * GetDeltaTime();

			if( (m_direction.x > 0 && capPos.x >= m_targetPosition.x) ||
				(m_direction.x < 0 && capPos.x <= m_targetPosition.x) ||
				(m_direction.z > 0 && capPos.z >= m_targetPosition.z) ||
				(m_direction.z < 0 && capPos.z <= m_targetPosition.z) )
			{
				m_isMoving = false;
				capPos = m_targetPosition;
			}
		}
		if( m_isTurning )
		{
			float angle = angleRotateCamera*3.0f * GetDeltaTime();


			if( m_rotateDir == rotateDirY::Left )
			{
				if( TargetRotateAngle2 >= 0.0f )
				{
					m_isTurning = false;
					m_rotateDir = rotateDirY::No;
					TargetRotateAngle2 = 0.0f;

					camera.x = camera.x.GetNormalize();
					camera.y = camera.y.GetNormalize();
					camera.z = camera.z.GetNormalize();
				}
				else
					TargetRotateAngle2 -= angle;
			}

			if( m_rotateDir == rotateDirY::Right )
			{
				if( TargetRotateAngle2 <= 0.0f )
				{
					m_isTurning = false;
					m_rotateDir = rotateDirY::No;
					TargetRotateAngle2 = 0.0f;
				}
				else
					TargetRotateAngle2 -= angle;
			}

			camera.x = Rotate(camera.x, angle, Vector3::Up);
			camera.y = Rotate(camera.y, angle, Vector3::Up);
			camera.z = Rotate(camera.z, angle, Vector3::Up);

			
		}
		camera.SetPosition(capPos);
	}
}

Matrix4 PlayerCamera::GetView()
{
	return camera.GetViewMatrix();
}