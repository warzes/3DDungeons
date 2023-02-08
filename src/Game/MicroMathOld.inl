#pragma once

//=============================================================================
// Vector3 Impl
//=============================================================================

inline const Vector3Old Vector3Old::Zero    = { 0.0f };
inline const Vector3Old Vector3Old::One     = { 1.0f };
inline const Vector3Old Vector3Old::Left    = { -1.0f, 0.0f, 0.0f };
inline const Vector3Old Vector3Old::Right   = { 1.0f, 0.0f, 0.0f };
inline const Vector3Old Vector3Old::Up      = { 0.0f, 1.0f, 0.0f };
inline const Vector3Old Vector3Old::Down    = { 0.0f,-1.0f, 0.0f };
inline const Vector3Old Vector3Old::Forward = { 0.0f, 0.0f, 1.0f };
inline const Vector3Old Vector3Old::Back    = { 0.0f, 0.0f, -1.0f };

inline float Vector3Old::GetLength() const
{
	return sqrtf(x * x + y * y + z * z);
}

inline float Vector3Old::GetLengthSquared() const
{
	return x * x + y * y + z * z;
}

inline Vector3Old Vector3Old::GetNormalize() const
{
	const float invLen = 1.0f / sqrtf(x * x + y * y + z * z);
	return { x * invLen, y * invLen, z * invLen };
}

inline float Distance(const Vector3Old& v1, const Vector3Old& v2)
{
	return (v1 - v2).GetLength();
}

inline float DotProduct(const Vector3Old& v1, const Vector3Old& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline Vector3Old CrossProduct(const Vector3Old& v1, const Vector3Old& v2)
{
	return {
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	};
}

inline Vector3Old Min(const Vector3Old& v1, const Vector3Old& v2)
{
	return {
		Min(v1.x, v2.x),
		Min(v1.y, v2.y),
		Min(v1.z, v2.z),
	};
}

inline Vector3Old Max(const Vector3Old& v1, const Vector3Old& v2)
{
	return {
		Max(v1.x, v2.x),
		Max(v1.y, v2.y),
		Max(v1.z, v2.z),
	};
}

inline Vector3Old Lerp(const Vector3Old& a, const Vector3Old& b, float x)
{
	return a + (b - a) * x;
}

inline Vector3Old Mix(const Vector3Old& u, const Vector3Old& v, float t)
{
	return u * (1.0f - t) + v * t;
}

inline Vector3Old Rotate(const Vector3Old& u, float angle, const Vector3Old& v)
{
	return *(Vector3Old*)&(Matrix4Old::Rotate(v, angle) * Vector4Old(u.x, u.y, u.z, 1.0f));
}

inline float Angle(const Vector3Old& v1, const Vector3Old& v2)
{
	return ::Acos(DotProduct(v1, v2) / (v1.GetLength() * v2.GetLength()));
}


//=============================================================================
// Vector4 Impl
//=============================================================================

inline float Vector4Old::GetLength() const
{
	return sqrtf(x * x + y * y + z * z + w * w);
}

inline float Vector4Old::GetLengthSquared() const
{
	return x * x + y * y + z * z + w * w;
}

inline Vector4Old Vector4Old::GetNormalize() const
{
	const float invLen = 1.0f / sqrtf(x * x + y * y + z * z + w * w);
	return { x * invLen, y * invLen, z * invLen, w * invLen };
}

inline float DotProduct(const Vector4Old& v1, const Vector4Old& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

//=============================================================================
// Quaternion Impl
//=============================================================================

const QuaternionOld Identity = { 0.0f, 0.0f, 0.0f, 1.0f };

inline QuaternionOld::QuaternionOld(float angle, const Vector3Old& axis)
{
	FromAngleAxis(angle, axis);
}

inline QuaternionOld::QuaternionOld(float ax, float ay, float az)
{
	FromEulerAngles(x, y, z);
}

inline QuaternionOld::QuaternionOld(const Vector3Old& start, const Vector3Old& end)
{
	FromRotationTo(start, end);
}

inline QuaternionOld::QuaternionOld(const Vector3Old& xAxis, const Vector3Old& yAxis, const Vector3Old& zAxis)
{
	FromAxes(xAxis, yAxis, zAxis);
}

inline QuaternionOld::QuaternionOld(const Matrix3Old& matrix)
{
	FromRotationMatrix(matrix);
}

inline QuaternionOld operator*(const QuaternionOld& u, const QuaternionOld& v)
{
	return {
		u.w * v.x + u.x * v.w + u.y * v.z - u.z * v.y,
		u.w * v.y + u.y * v.w + u.z * v.x - u.x * v.z,
		u.w * v.z + u.z * v.w + u.x * v.y - u.y * v.x,
		u.w * v.w - u.x * v.x - u.y * v.y - u.z * v.z
	};
}

inline Vector3Old QuaternionOld::operator*(const Vector3Old& v) const
{
	const Vector3Old qvec(x, y, z);
	Vector3Old uv = CrossProduct(qvec, v);
	Vector3Old uuv = CrossProduct(qvec, uv);
	return v + 2.0f * (uv * w + uuv);
}

inline void QuaternionOld::FromAngleAxis(float angle, const Vector3Old& axis)
{
	Vector3Old normAxis = axis.GetNormalize();
	angle *= DEG2RAD_2;
	const float sinAngle = sinf(angle);
	const float cosAngle = cosf(angle);

	x = normAxis.x * sinAngle;
	y = normAxis.y * sinAngle;
	z = normAxis.z * sinAngle;
	w = cosAngle;
}

inline void QuaternionOld::FromEulerAngles(float x_, float y_, float z_)
{
	// Order of rotations: Z first, then X, then Y (mimics typical FPS camera with gimbal lock at top/bottom)
	x_ *= DEG2RAD_2;
	y_ *= DEG2RAD_2;
	z_ *= DEG2RAD_2;
	const float sinX = sinf(x_);
	const float cosX = cosf(x_);
	const float sinY = sinf(y_);
	const float cosY = cosf(y_);
	const float sinZ = sinf(z_);
	const float cosZ = cosf(z_);

	x = cosY * sinX * cosZ + sinY * cosX * sinZ;
	y = sinY * cosX * cosZ - cosY * sinX * sinZ;
	z = cosY * cosX * sinZ - sinY * sinX * cosZ;
	w = cosY * cosX * cosZ + sinY * sinX * sinZ;
}

inline void QuaternionOld::FromRotationTo(const Vector3Old& start, const Vector3Old& end)
{
	const Vector3Old normStart = start.GetNormalize();
	const Vector3Old normEnd = end.GetNormalize();
	const float d = DotProduct(normStart, normEnd);

	if( d > -1.0f + EPSILON )
	{
		const Vector3Old c = CrossProduct(normStart, normEnd);
		const float s = sqrtf((1.0f + d) * 2.0f);
		const float invS = 1.0f / s;

		x = c.x * invS;
		y = c.y * invS;
		z = c.z * invS;
		w = 0.5f * s;
	}
	else
	{
		Vector3Old axis = CrossProduct(Vector3Old::Right, normStart);
		if( axis.GetLength() < EPSILON )
			axis = CrossProduct(Vector3Old::Up, normStart);

		FromAngleAxis(180.0f, axis);
	}
}

inline void QuaternionOld::FromAxes(const Vector3Old& xAxis, const Vector3Old& yAxis, const Vector3Old& zAxis)
{
	Matrix3Old matrix(
		xAxis.x, yAxis.x, zAxis.x,
		xAxis.y, yAxis.y, zAxis.y,
		xAxis.z, yAxis.z, zAxis.z
	);

	FromRotationMatrix(matrix);
}

inline void QuaternionOld::FromRotationMatrix(const Matrix3Old& matrix)
{
	const float t = matrix[0] + matrix[4] + matrix[8];

	if( t > 0.0f )
	{
		const float invS = 0.5f / sqrtf(1.0f + t);

		x = (matrix[7] - matrix[5]) * invS;
		y = (matrix[2] - matrix[6]) * invS;
		z = (matrix[3] - matrix[1]) * invS;
		w = 0.25f / invS;
	}
	else
	{
		if( matrix[0] > matrix[4] && matrix[0] > matrix[8] )
		{
			const float invS = 0.5f / sqrtf(1.0f + matrix[0] - matrix[4] - matrix[8]);

			x = 0.25f / invS;
			y = (matrix[1] + matrix[3]) * invS;
			z = (matrix[6] + matrix[2]) * invS;
			w = (matrix[7] - matrix[5]) * invS;
		}
		else if( matrix[4] > matrix[8] )
		{
			const float invS = 0.5f / sqrtf(1.0f + matrix[4] - matrix[0] - matrix[8]);

			x = (matrix[1] + matrix[3]) * invS;
			y = 0.25f / invS;
			z = (matrix[5] + matrix[7]) * invS;
			w = (matrix[2] - matrix[6]) * invS;
		}
		else
		{
			const float invS = 0.5f / sqrtf(1.0f + matrix[8] - matrix[0] - matrix[4]);

			x = (matrix[2] + matrix[6]) * invS;
			y = (matrix[5] + matrix[7]) * invS;
			z = 0.25f / invS;
			w = (matrix[3] - matrix[1]) * invS;
		}
	}
}

inline bool QuaternionOld::FromLookRotation(const Vector3Old& direction, const Vector3Old& upDirection)
{
	QuaternionOld ret;
	Vector3Old forward = direction.GetNormalize();

	Vector3Old v = CrossProduct(forward, upDirection);
	// If direction & upDirection are parallel and crossproduct becomes zero, use FromRotationTo() fallback
	if( v.GetLengthSquared() >= EPSILON )
	{
		v = v.GetNormalize();
		Vector3Old up = CrossProduct(v, forward);
		Vector3Old right = CrossProduct(up, forward);
		ret.FromAxes(right, up, forward);
	}
	else
		ret.FromRotationTo(Vector3Old::Forward, forward);

	if( !ret.IsNaN() )
	{
		(*this) = ret;
		return true;
	}
	else
		return false;
}

inline float QuaternionOld::GetLength() const
{
	return sqrtf(x * x + y * y + z * z + w * w);
}


inline float QuaternionOld::GetLengthSquared() const
{
	return x * x + y * y + z * z + w * w;
}

inline QuaternionOld QuaternionOld::GetNormalize() const
{
	const float invLen = 1.0f / sqrtf(x * x + y * y + z * z + w * w);
	return { x * invLen, y * invLen, z * invLen, w * invLen };
}

inline QuaternionOld QuaternionOld::Inverse() const
{
	const float lenSquared = GetLengthSquared();
	if( lenSquared == 1.0f )
		return Conjugate();
	else if( lenSquared >= EPSILON )
		return Conjugate() * (1.0f / lenSquared);
	else
		return Identity;
}

inline Vector3Old QuaternionOld::EulerAngles() const
{
	// Derivation from http://www.geometrictools.com/Documentation/EulerAngles.pdf
	// Order of rotations: Z first, then X, then Y
	const float check = 2.0f * (w * x - y * z);

	if( check < -0.995f )
	{
		return Vector3Old(
			-90.0f,
			0.0f,
			-atan2f(2.0f * (x * z - w * y), 1.0f - 2.0f * (y * y + z * z)) * RAD2DEG
		);
	}
	else if( check > 0.995f )
	{
		return Vector3Old(
			90.0f,
			0.0f,
			atan2f(2.0f * (x * z - w * y), 1.0f - 2.0f * (y * y + z * z)) * RAD2DEG
		);
	}
	else
	{
		return Vector3Old(
			asinf(check) * RAD2DEG,
			atan2f(2.0f * (x * z + w * y), 1.0f - 2.0f * (x * x + y * y)) * RAD2DEG,
			atan2f(2.0f * (x * y + w * z), 1.0f - 2.0f * (x * x + z * z)) * RAD2DEG
		);
	}
}

inline float QuaternionOld::YawAngle() const
{
	return EulerAngles().y;
}

inline float QuaternionOld::PitchAngle() const
{
	return EulerAngles().x;
}

inline float QuaternionOld::RollAngle() const
{
	return EulerAngles().z;
}

inline Matrix3Old QuaternionOld::RotationMatrix() const
{
	return {
		1.0f - 2.0f * y * y - 2.0f * z * z,
		2.0f * x * y - 2.0f * w * z,
		2.0f * x * z + 2.0f * w * y,
		2.0f * x * y + 2.0f * w * z,
		1.0f - 2.0f * x * x - 2.0f * z * z,
		2.0f * y * z - 2.0f * w * x,
		2.0f * x * z - 2.0f * w * y,
		2.0f * y * z + 2.0f * w * x,
		1.0f - 2.0f * x * x - 2.0f * y * y
	};
}

inline QuaternionOld QuaternionOld::Slerp(QuaternionOld rhs, float t) const
{
	float cosAngle = DotProduct(*this, rhs);
	// Enable shortest path rotation
	if( cosAngle < 0.0f )
	{
		cosAngle = -cosAngle;
		rhs = -rhs;
	}

	const float angle = acosf(cosAngle);
	const float sinAngle = sinf(angle);
	
	float t1, t2;
	if( sinAngle > 0.001f )
	{
		const float invSinAngle = 1.0f / sinAngle;
		t1 = sinf((1.0f - t) * angle) * invSinAngle;
		t2 = sinf(t * angle) * invSinAngle;
	}
	else
	{
		t1 = 1.0f - t;
		t2 = t;
	}

	return *this * t1 + rhs * t2;
}

inline QuaternionOld QuaternionOld::Nlerp(QuaternionOld rhs, float t, bool shortestPath) const
{
	QuaternionOld result;
	const float fCos = DotProduct(*this, rhs);
	if( fCos < 0.0f && shortestPath )
		result = (*this) + (((-rhs) - (*this)) * t);
	else
		result = (*this) + ((rhs - (*this)) * t);
	return result.GetNormalize();
}

inline float DotProduct(const QuaternionOld& v1, const QuaternionOld& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

//=============================================================================
// Matrix3 Impl
//=============================================================================

inline constexpr Matrix3Old::Matrix3Old(const float* f)
{
	m[0] = f[0]; m[1] = f[1]; m[2] = f[2];
	m[3] = f[3]; m[4] = f[4]; m[5] = f[5];
	m[6] = f[6]; m[7] = f[7]; m[8] = f[8];
}

//inline constexpr Matrix3::Matrix3(const Vector3& col1, const Vector3& col2, const Vector3& col3)
//{
//	m[0] = col1.x; m[3] = col2.x; m[6] = col3.x;
//	m[1] = col1.y; m[4] = col2.y; m[7] = col3.y;
//	m[2] = col1.z; m[5] = col2.z; m[8] = col3.z;
//}

inline constexpr Matrix3Old::Matrix3Old(
	float m0, float m1, float m2,
	float m3, float m4, float m5,
	float m6, float m7, float m8)
{
	m[0] = m0; m[1] = m1; m[2] = m2;
	m[3] = m3; m[4] = m4; m[5] = m5;
	m[6] = m6; m[7] = m7; m[8] = m8;
}
//
//inline Matrix3::Matrix3(const Matrix4& M)
//{
//	m[0] = M[0]; m[3] = M[4]; m[6] = M[8];
//	m[1] = M[1]; m[4] = M[5]; m[7] = M[9];
//	m[2] = M[2]; m[5] = M[6]; m[8] = M[10];
//}

inline Matrix3Old operator+(const Matrix3Old& m1, const Matrix3Old& m2)
{
	return {
		m1[0] + m2[0], m1[1] + m2[1], m1[2] + m2[2],
		m1[3] + m2[3], m1[4] + m2[4], m1[5] + m2[5],
		m1[6] + m2[6], m1[7] + m2[7], m1[8] + m2[8]
	};
}

inline Matrix3Old operator-(const Matrix3Old& m1, const Matrix3Old& m2)
{
	return {
		m1[0] - m2[0], m1[1] - m2[1], m1[2] - m2[2],
		m1[3] - m2[3], m1[4] - m2[4], m1[5] - m2[5],
		m1[6] - m2[6], m1[7] - m2[7], m1[8] - m2[8]
	};
}

inline Matrix3Old operator*(float f, const Matrix3Old & m)
{
	return m * f;
}

inline Matrix3Old operator*(const Matrix3Old& m, float f)
{
	return {
		m[0] * f, m[1] * f, m[2] * f,
		m[3] * f, m[4] * f, m[5] * f,
		m[6] * f, m[7] * f, m[8] * f
	};
}

inline Vector3Old operator*(const Matrix3Old& m, const Vector3Old& u)
{
	return {
		m[0] * u.x + m[1] * u.y + m[2] * u.z,
		m[3] * u.x + m[4] * u.y + m[5] * u.z,
		m[6] * u.x + m[7] * u.y + m[8] * u.z
	};
}


inline Matrix3Old operator*(const Matrix3Old& m1, const Matrix3Old& m2)
{
	return {
		m1[0] * m2[0] + m1[1] * m2[3] + m1[2] * m2[6],
		m1[0] * m2[1] + m1[1] * m2[4] + m1[2] * m2[7],
		m1[0] * m2[2] + m1[1] * m2[5] + m1[2] * m2[8],
		m1[3] * m2[0] + m1[4] * m2[3] + m1[5] * m2[6],
		m1[3] * m2[1] + m1[4] * m2[4] + m1[5] * m2[7],
		m1[3] * m2[2] + m1[4] * m2[5] + m1[5] * m2[8],
		m1[6] * m2[0] + m1[7] * m2[3] + m1[8] * m2[6],
		m1[6] * m2[1] + m1[7] * m2[4] + m1[8] * m2[7],
		m1[6] * m2[2] + m1[7] * m2[5] + m1[8] * m2[8]
	};
}

inline constexpr void Matrix3Old::Set(const float* f)
{
	m[0] = f[0]; m[1] = f[1]; m[2] = f[2];
	m[3] = f[3]; m[4] = f[4]; m[5] = f[5];
	m[6] = f[6]; m[7] = f[7]; m[8] = f[8];
}

inline constexpr void Matrix3Old::Set(
	float m0, float m1, float m2,
	float m3, float m4, float m5,
	float m6, float m7, float m8)
{
	m[0] = m0; m[1] = m1; m[2] = m2;
	m[3] = m3; m[4] = m4; m[5] = m5;
	m[6] = m6; m[7] = m7; m[8] = m8;
}

inline constexpr void Matrix3Old::Set(const Matrix3Old& M)
{
	m[0] = M[0]; m[1] = M[1]; m[2] = M[2];
	m[3] = M[3]; m[4] = M[4]; m[5] = M[5];
	m[6] = M[6]; m[7] = M[7]; m[8] = M[8];
}

inline void Matrix3Old::SetScale(const Vector3Old& scale)
{
	m[0] = scale.x;
	m[4] = scale.y;
	m[8] = scale.z;
}

inline void Matrix3Old::SetScale(float scale)
{
	m[0] = scale;
	m[4] = scale;
	m[8] = scale;
}

inline Vector3Old Matrix3Old::GetScale() const
{
	return {
		sqrtf(m[0] * m[0] + m[3] * m[3] + m[6] * m[6]),
		sqrtf(m[1] * m[1] + m[4] * m[4] + m[7] * m[7]),
		sqrtf(m[2] * m[2] + m[5] * m[5] + m[8] * m[8])
	};
}

inline Matrix3Old Matrix3Old::Scaled(const Vector3Old & scale) const
{
	return {
		m[0] * scale.x, m[1] * scale.y, m[2] * scale.z,
		m[3] * scale.x, m[4] * scale.y, m[5] * scale.z,
		m[6] * scale.x, m[7] * scale.y, m[8] * scale.z
	};
}

inline Matrix3Old Matrix3Old::Inverse() const
{
	const float det =
		m[0] * m[4] * m[8] +
		m[3] * m[7] * m[2] +
		m[6] * m[1] * m[5] -
		m[6] * m[4] * m[2] -
		m[3] * m[1] * m[8] -
		m[0] * m[7] * m[5];
	const float invDet = 1.0f / det;

	return {
		 (m[4] * m[8] - m[7] * m[5]) * invDet,
		-(m[1] * m[8] - m[7] * m[2]) * invDet,
		 (m[1] * m[5] - m[4] * m[2]) * invDet,
		-(m[3] * m[8] - m[6] * m[5]) * invDet,
		 (m[0] * m[8] - m[6] * m[2]) * invDet,
		-(m[0] * m[5] - m[3] * m[2]) * invDet,
		 (m[3] * m[7] - m[6] * m[4]) * invDet,
		-(m[0] * m[7] - m[6] * m[1]) * invDet,
		 (m[0] * m[4] - m[3] * m[1]) * invDet
	};
}

inline Matrix3Old Matrix3Old::Transpose() const
{
	return {
		m[0], m[3], m[6],
		m[1], m[4], m[7],
		m[2], m[5], m[8]
	};
}

//=============================================================================
// Matrix3x4 Impl
//=============================================================================

inline constexpr Matrix3x4Old::Matrix3x4Old(const float* data)
{
	m[0] = data[0]; m[1] = data[1]; m[ 2] = data[ 2]; m[ 3] = data[ 3];
	m[4] = data[4]; m[5] = data[5]; m[ 6] = data[ 6]; m[ 7] = data[ 7];
	m[8] = data[8]; m[9] = data[9]; m[10] = data[10]; m[11] = data[11];
}

inline constexpr Matrix3x4Old::Matrix3x4Old(const Matrix3Old& matrix)
{
	m[0] = matrix[0]; m[1] = matrix[1]; m[ 2] = matrix[2]; m[ 3] = 0.0f;
	m[4] = matrix[3]; m[5] = matrix[4]; m[ 6] = matrix[5]; m[ 7] = 0.0f;
	m[8] = matrix[6]; m[9] = matrix[7]; m[10] = matrix[8]; m[11] = 0.0f;
}

inline constexpr Matrix3x4Old::Matrix3x4Old(const Matrix4Old& matrix)
{
	m[0] = matrix[0]; m[1] = matrix[1]; m[ 2] = matrix[ 2]; m[ 3] = matrix[ 3];
	m[4] = matrix[4]; m[5] = matrix[5]; m[ 6] = matrix[ 6]; m[ 7] = matrix[ 7];
	m[8] = matrix[8]; m[9] = matrix[9]; m[10] = matrix[10]; m[11] = matrix[11];
}

inline Matrix3x4Old::Matrix3x4Old(
	float v00, float v01, float v02, float v03,
	float v10, float v11, float v12, float v13,
	float v20, float v21, float v22, float v23)
{
	m[0] = v00; m[1] = v01; m[ 2] = v02; m[ 3] = v03;
	m[4] = v10; m[5] = v11; m[ 6] = v12; m[ 7] = v13;
	m[8] = v20; m[9] = v21; m[10] = v22; m[11] = v23;
}

inline Matrix3x4Old::Matrix3x4Old(const Vector3Old& translation, const QuaternionOld& rotation, float scale)
{
	SetRotation(rotation.RotationMatrix() * scale);
	SetTranslation(translation);
}

inline Matrix3x4Old::Matrix3x4Old(const Vector3Old& translation, const QuaternionOld& rotation, const Vector3Old& scale)
{
	SetRotation(rotation.RotationMatrix().Scaled(scale));
	SetTranslation(translation);
}

inline Matrix3x4Old& Matrix3x4Old::operator=(const Matrix3Old& rhs)
{
	m[0] = rhs[0]; m[1] = rhs[1]; m[ 2] = rhs[2]; m[ 3] = 0.0f;
	m[4] = rhs[3]; m[5] = rhs[4]; m[ 6] = rhs[5]; m[ 7] = 0.0f;
	m[8] = rhs[6]; m[9] = rhs[7]; m[10] = rhs[8]; m[11] = 0.0f;
	return *this;
}

inline Matrix3x4Old& Matrix3x4Old::operator=(const Matrix4Old& rhs)
{
	m[0] = rhs[0]; m[1] = rhs[1]; m[ 2] = rhs[ 2]; m[ 3] = rhs[ 3];
	m[4] = rhs[4]; m[5] = rhs[5]; m[ 6] = rhs[ 6]; m[ 7] = rhs[ 7];
	m[8] = rhs[8]; m[9] = rhs[9]; m[10] = rhs[10]; m[11] = rhs[11];
	return *this;
}

inline Matrix3x4Old operator+(const Matrix3x4Old& m1, const Matrix3x4Old& m2)
{
	return {
		m1[0] + m2[0], m1[1] + m2[1], m1[ 2] + m2[ 2], m1[ 3] + m2[ 3],
		m1[4] + m2[4], m1[5] + m2[5], m1[ 6] + m2[ 6], m1[ 7] + m2[ 7],
		m1[8] + m2[8], m1[9] + m2[9], m1[10] + m2[10], m1[11] + m2[11]
	};
}

inline Matrix3x4Old operator-(const Matrix3x4Old& m1, const Matrix3x4Old& m2)
{
	return {
		m1[0] - m2[0], m1[1] - m2[1], m1[ 2] - m2[ 2], m1[ 3] - m2[ 3],
		m1[4] - m2[4], m1[5] - m2[5], m1[ 6] - m2[ 6], m1[ 7] - m2[ 7],
		m1[8] - m2[8], m1[9] - m2[9], m1[10] - m2[10], m1[11] - m2[11]
	};
}

inline Matrix3x4Old operator*(float f, const Matrix3x4Old& m)
{
	return m * f;
}

inline Matrix3x4Old operator*(const Matrix3x4Old& m, float f)
{
	return {
		m[0] * f, m[1] * f, m[ 2] * f, m[ 3] * f,
		m[4] * f, m[5] * f, m[ 6] * f, m[ 7] * f,
		m[8] * f, m[9] * f, m[10] * f, m[11] * f
	};
}

inline Vector3Old operator*(const Matrix3x4Old& m, const Vector3Old& v)
{
	return {
		(m[0] * v.x + m[1] * v.y + m[ 2] * v.z + m[ 3]),
		(m[4] * v.x + m[5] * v.y + m[ 6] * v.z + m[ 7]),
		(m[8] * v.x + m[9] * v.y + m[10] * v.z + m[11])
	};
}

inline Vector3Old operator*(const Matrix3x4Old& m, const Vector4Old& v)
{
	return {
		(m[0] * v.x + m[1] * v.y + m[ 2] * v.z + m[ 3] * v.w),
		(m[4] * v.x + m[5] * v.y + m[ 6] * v.z + m[ 7] * v.w),
		(m[8] * v.x + m[9] * v.y + m[10] * v.z + m[11] * v.w)
	};
}

inline Matrix3x4Old operator*(const Matrix3x4Old& m1, const Matrix3x4Old& m2)
{
	return {
		m1[0] * m2[0] + m1[1] * m2[4] + m1[ 2] * m2[ 8],
		m1[0] * m2[1] + m1[1] * m2[5] + m1[ 2] * m2[ 9],
		m1[0] * m2[2] + m1[1] * m2[6] + m1[ 2] * m2[10],
		m1[0] * m2[3] + m1[1] * m2[7] + m1[ 2] * m2[11] + m1[ 3],

		m1[4] * m2[0] + m1[5] * m2[4] + m1[ 6] * m2[ 8],
		m1[4] * m2[1] + m1[5] * m2[5] + m1[ 6] * m2[ 9],
		m1[4] * m2[2] + m1[5] * m2[6] + m1[ 6] * m2[10],
		m1[4] * m2[3] + m1[5] * m2[7] + m1[ 6] * m2[11] + m1[ 7],

		m1[8] * m2[0] + m1[9] * m2[4] + m1[10] * m2[ 8],
		m1[8] * m2[1] + m1[9] * m2[5] + m1[10] * m2[ 9],
		m1[8] * m2[2] + m1[9] * m2[6] + m1[10] * m2[10],
		m1[8] * m2[3] + m1[9] * m2[7] + m1[10] * m2[11] + m1[11]
	};
}

inline Matrix4Old operator*(const Matrix3x4Old& m1, const Matrix4Old& m2)
{
	return {
		m1[0] * m2[0] + m1[1] * m2[4] + m1[ 2] * m2[ 8] + m1[ 3] * m2[12],
		m1[0] * m2[1] + m1[1] * m2[5] + m1[ 2] * m2[ 9] + m1[ 3] * m2[13],
		m1[0] * m2[2] + m1[1] * m2[6] + m1[ 2] * m2[10] + m1[ 3] * m2[14],
		m1[0] * m2[3] + m1[1] * m2[7] + m1[ 2] * m2[11] + m1[ 3] * m2[15],

		m1[4] * m2[0] + m1[5] * m2[4] + m1[ 6] * m2[ 8] + m1[ 7] * m2[12],
		m1[4] * m2[1] + m1[5] * m2[5] + m1[ 6] * m2[ 9] + m1[ 7] * m2[13],
		m1[4] * m2[2] + m1[5] * m2[6] + m1[ 6] * m2[10] + m1[ 7] * m2[14],
		m1[4] * m2[3] + m1[5] * m2[7] + m1[ 6] * m2[11] + m1[ 7] * m2[15],

		m1[8] * m2[0] + m1[9] * m2[4] + m1[10] * m2[ 8] + m1[11] * m2[12],
		m1[8] * m2[1] + m1[9] * m2[5] + m1[10] * m2[ 9] + m1[11] * m2[13],
		m1[8] * m2[2] + m1[9] * m2[6] + m1[10] * m2[10] + m1[11] * m2[14],
		m1[8] * m2[3] + m1[9] * m2[7] + m1[10] * m2[11] + m1[11] * m2[15],

		m2[12],
		m2[13],
		m2[14],
		m2[15]
	};
}

inline Matrix4Old operator*(const Matrix4Old& lhs, const Matrix3x4Old& rhs)
{
	return {
		lhs[0] * rhs[0] + lhs[1] * rhs[4] + lhs[2] * rhs[8],
		lhs[0] * rhs[1] + lhs[1] * rhs[5] + lhs[2] * rhs[9],
		lhs[0] * rhs[2] + lhs[1] * rhs[6] + lhs[2] * rhs[10],
		lhs[0] * rhs[3] + lhs[1] * rhs[7] + lhs[2] * rhs[11] + lhs[3],
		lhs[4] * rhs[0] + lhs[5] * rhs[4] + lhs[6] * rhs[8],
		lhs[4] * rhs[1] + lhs[5] * rhs[5] + lhs[6] * rhs[9],
		lhs[4] * rhs[2] + lhs[5] * rhs[6] + lhs[6] * rhs[10],
		lhs[4] * rhs[3] + lhs[5] * rhs[7] + lhs[6] * rhs[11] + lhs[7],
		lhs[8] * rhs[0] + lhs[9] * rhs[4] + lhs[10] * rhs[8],
		lhs[8] * rhs[1] + lhs[9] * rhs[5] + lhs[10] * rhs[9],
		lhs[8] * rhs[2] + lhs[9] * rhs[6] + lhs[10] * rhs[10],
		lhs[8] * rhs[3] + lhs[9] * rhs[7] + lhs[10] * rhs[11] + lhs[11],
		lhs[12] * rhs[0] + lhs[13] * rhs[4] + lhs[14] * rhs[8],
		lhs[12] * rhs[1] + lhs[13] * rhs[5] + lhs[14] * rhs[9],
		lhs[12] * rhs[2] + lhs[13] * rhs[6] + lhs[14] * rhs[10],
		lhs[12] * rhs[3] + lhs[13] * rhs[7] + lhs[14] * rhs[11] + lhs[15]
	};
}

inline void Matrix3x4Old::SetTranslation(const Vector3Old& translation)
{
	m[ 3] = translation.x;
	m[ 7] = translation.y;
	m[11] = translation.z;
}

inline void Matrix3x4Old::SetRotation(const Matrix3Old& rotation)
{
	m[0] = rotation[0]; m[1] = rotation[1]; m[ 2] = rotation[2];
	m[4] = rotation[3]; m[5] = rotation[4]; m[ 6] = rotation[5];
	m[8] = rotation[6]; m[9] = rotation[7]; m[10] = rotation[8];
}

inline void Matrix3x4Old::SetScale(const Vector3Old& scale)
{
	m[ 0] = scale.x;
	m[ 5] = scale.y;
	m[10] = scale.z;
}

inline void Matrix3x4Old::SetScale(float scale)
{
	m[ 0] = scale;
	m[ 5] = scale;
	m[10] = scale;
}

inline Matrix3Old Matrix3x4Old::ToMatrix3() const
{
	return {
		m[0], m[1], m[ 2],
		m[4], m[5], m[ 6],
		m[8], m[9], m[10]
	};
}

inline Matrix4Old Matrix3x4Old::ToMatrix4() const
{
	return {
		m[0], m[1], m[ 2], m[ 3],
		m[4], m[5], m[ 6], m[ 7],
		m[8], m[9], m[10], m[11],
		0.0f, 0.0f, 0.0f,  1.0f
	};
}

inline Matrix3Old Matrix3x4Old::RotationMatrix() const
{
	Vector3Old invScale(
		1.0f / sqrtf(m[0] * m[0] + m[4] * m[4] + m[ 8] * m[ 8]),
		1.0f / sqrtf(m[1] * m[1] + m[5] * m[5] + m[ 9] * m[ 9]),
		1.0f / sqrtf(m[2] * m[2] + m[6] * m[6] + m[10] * m[10])
	);

	return ToMatrix3().Scaled(invScale);
}

inline Vector3Old Matrix3x4Old::GetTranslation() const
{
	return { m[3], m[7], m[11] };
}

inline Vector3Old Matrix3x4Old::GetScale() const
{
	return {
		sqrtf(m[0] * m[0] + m[4] * m[4] + m[ 8] * m[ 8]),
		sqrtf(m[1] * m[1] + m[5] * m[5] + m[ 9] * m[ 9]),
		sqrtf(m[2] * m[2] + m[6] * m[6] + m[10] * m[10])
	};
}

inline void Matrix3x4Old::Decompose(Vector3Old & translation, QuaternionOld & rotation, Vector3Old & scale) const
{
	translation.x = m[ 3];
	translation.y = m[ 7];
	translation.z = m[11];

	scale.x = sqrtf(m[0] * m[0] + m[4] * m[4] + m[ 8] * m[ 8]);
	scale.y = sqrtf(m[1] * m[1] + m[5] * m[5] + m[ 9] * m[ 9]);
	scale.z = sqrtf(m[2] * m[2] + m[6] * m[6] + m[10] * m[10]);

	Vector3Old invScale(1.0f / scale.x, 1.0f / scale.y, 1.0f / scale.z);
	rotation = QuaternionOld(ToMatrix3().Scaled(invScale));
}

inline Matrix3x4Old Matrix3x4Old::Inverse() const
{
	const float det = 
		m[0] * m[5] * m[10] +
		m[4] * m[9] * m[2] +
		m[8] * m[1] * m[6] -
		m[8] * m[5] * m[2] -
		m[4] * m[1] * m[10] -
		m[0] * m[9] * m[6];
	const float invDet = 1.0f / det;

	Matrix3x4Old ret;
	ret[0] = (m[5] * m[10] - m[9] * m[6]) * invDet;
	ret[1] = -(m[1] * m[10] - m[9] * m[2]) * invDet;
	ret[2] = (m[1] * m[6] - m[5] * m[2]) * invDet;
	ret[3] = -(m[3] * ret[0] + m[7] * ret[1] + m[11] * ret[2]);
	ret[4] = -(m[4] * m[10] - m[8] * m[6]) * invDet;
	ret[5] = (m[0] * m[10] - m[8] * m[2]) * invDet;
	ret[6] = -(m[0] * m[6] - m[4] * m[2]) * invDet;
	ret[7] = -(m[3] * ret[4] + m[7] * ret[5] + m[11] * ret[6]);
	ret[8] = (m[4] * m[9] - m[8] * m[5]) * invDet;
	ret[9] = -(m[0] * m[9] - m[8] * m[1]) * invDet;
	ret[10] = (m[0] * m[5] - m[4] * m[1]) * invDet;
	ret[11] = -(m[3] * ret[8] + m[7] * ret[9] + m[11] * ret[10]);
	return ret;
}

//=============================================================================
// Matrix4 Impl
//=============================================================================

inline constexpr Matrix4Old::Matrix4Old(const float* f)
{
	m[0] = f[0];  m[1] = f[1];  m[2] = f[2];  m[3] = f[3];
	m[4] = f[4];  m[5] = f[5];  m[6] = f[6];  m[7] = f[7];
	m[8] = f[8];  m[9] = f[9];  m[10] = f[10]; m[11] = f[11];
	m[12] = f[12]; m[13] = f[13]; m[14] = f[14]; m[15] = f[15];
}

inline constexpr Matrix4Old::Matrix4Old(
	float  m0, float  m1, float  m2, float  m3,
	float  m4, float  m5, float  m6, float  m7,
	float  m8, float  m9, float m10, float m11,
	float m12, float m13, float m14, float m15)
{
	m[0] = m0;  m[1] = m1;  m[2] = m2;  m[3] = m3;
	m[4] = m4;  m[5] = m5;  m[6] = m6;  m[7] = m7;
	m[8] = m8;  m[9] = m9;  m[10] = m10; m[11] = m11;
	m[12] = m12; m[13] = m13; m[14] = m14; m[15] = m15;
}

inline constexpr Matrix4Old::Matrix4Old(const Matrix3Old& mat)
{
	m[0] = mat[0]; m[4] = mat[3]; m[ 8] = mat[6]; m[12] = 0.0f;
	m[1] = mat[1]; m[5] = mat[4]; m[ 9] = mat[7]; m[13] = 0.0f;
	m[2] = mat[2]; m[6] = mat[5]; m[10] = mat[8]; m[14] = 0.0f;
	m[3] = 0.0f;   m[7] = 0.0f;   m[11] = 0.0f;   m[15] = 1.0f;
}

inline constexpr Matrix4Old::Matrix4Old(const Matrix3x4Old& mat)
{
	m[0] = mat[0]; m[4] = mat[4]; m[ 8] = mat[ 8]; m[12] = 0.0f;
	m[1] = mat[1]; m[5] = mat[5]; m[ 9] = mat[ 9]; m[13] = 0.0f;
	m[2] = mat[2]; m[6] = mat[6]; m[10] = mat[10]; m[14] = 0.0f;
	m[3] = mat[3]; m[7] = mat[7]; m[11] = mat[11]; m[15] = 1.0f;
}

inline Matrix4Old& Matrix4Old::operator=(const Matrix3Old& rhs)
{
	m[ 0] = rhs[0]; m[ 1] = rhs[1]; m[ 2] = rhs[2]; m[ 3] = 0.0f;
	m[ 4] = rhs[3]; m[ 5] = rhs[4]; m[ 6] = rhs[5]; m[ 7] = 0.0f;
	m[ 8] = rhs[6]; m[ 9] = rhs[7]; m[10] = rhs[8]; m[11] = 0.0f;
	m[12] = 0.0f;   m[13] = 0.0f;   m[14] = 0.0f;   m[15] = 1.0f;
	return *this;
}

inline Matrix4Old operator+(const Matrix4Old& m1, const Matrix4Old& m2)
{
	return {
		m1[0] + m2[0], m1[1] + m2[1], m1[2] + m2[2], m1[3] + m2[3],
		m1[4] + m2[4], m1[5] + m2[5], m1[6] + m2[6], m1[7] + m2[7],
		m1[8] + m2[8], m1[9] + m2[9], m1[10] + m2[10], m1[11] + m2[11],
		m1[12] + m2[12], m1[13] + m2[13], m1[14] + m2[14], m1[15] + m2[15]
	};
}

inline Matrix4Old operator-(const Matrix4Old& m1, const Matrix4Old& m2)
{
	return {
		m1[0] - m2[0], m1[1] - m2[1], m1[2] - m2[2], m1[3] - m2[3],
		m1[4] - m2[4], m1[5] - m2[5], m1[6] - m2[6], m1[7] - m2[7],
		m1[8] - m2[8], m1[9] - m2[9], m1[10] - m2[10], m1[11] - m2[11],
		m1[12] - m2[12], m1[13] - m2[13], m1[14] - m2[14], m1[15] - m2[15]
	};
}

inline Matrix4Old operator*(const Matrix4Old& m, float f)
{
	return {
		m[0] * f, m[1] * f, m[2] * f, m[3] * f,
		m[4] * f, m[5] * f, m[6] * f, m[7] * f,
		m[8] * f, m[9] * f, m[10] * f, m[11] * f,
		m[12] * f, m[13] * f, m[14] * f, m[15] * f
	};
}

inline Matrix4Old operator*(float f, const Matrix4Old& m)
{
	return m * f;
}

inline Vector3Old operator*(const Matrix4Old& m, const Vector3Old& rhs)
{
	float invW = 1.0f / (m[12] * rhs.x + m[13] * rhs.y + m[14] * rhs.z + m[15]);

	return {
		(m[0] * rhs.x + m[1] * rhs.y + m[2] * rhs.z + m[3]) * invW,
		(m[4] * rhs.x + m[5] * rhs.y + m[6] * rhs.z + m[7]) * invW,
		(m[8] * rhs.x + m[9] * rhs.y + m[10] * rhs.z + m[11]) * invW
	};
}

inline Vector4Old operator*(const Matrix4Old& m, const Vector4Old& rhs)
{
	return {
		m[0] * rhs.x + m[1] * rhs.y + m[2] * rhs.z + m[3] * rhs.w,
		m[4] * rhs.x + m[5] * rhs.y + m[6] * rhs.z + m[7] * rhs.w,
		m[8] * rhs.x + m[9] * rhs.y + m[10] * rhs.z + m[11] * rhs.w,
		m[12] * rhs.x + m[13] * rhs.y + m[14] * rhs.z + m[15] * rhs.w
	};
}

inline Matrix4Old operator*(const Matrix4Old& m1, const Matrix4Old& m2)
{
	return {
		m1[0] * m2[0] + m1[1] * m2[4] + m1[2] * m2[8] + m1[3] * m2[12],
		m1[0] * m2[1] + m1[1] * m2[5] + m1[2] * m2[9] + m1[3] * m2[13],
		m1[0] * m2[2] + m1[1] * m2[6] + m1[2] * m2[10] + m1[3] * m2[14],
		m1[0] * m2[3] + m1[1] * m2[7] + m1[2] * m2[11] + m1[3] * m2[15],
		m1[4] * m2[0] + m1[5] * m2[4] + m1[6] * m2[8] + m1[7] * m2[12],
		m1[4] * m2[1] + m1[5] * m2[5] + m1[6] * m2[9] + m1[7] * m2[13],
		m1[4] * m2[2] + m1[5] * m2[6] + m1[6] * m2[10] + m1[7] * m2[14],
		m1[4] * m2[3] + m1[5] * m2[7] + m1[6] * m2[11] + m1[7] * m2[15],
		m1[8] * m2[0] + m1[9] * m2[4] + m1[10] * m2[8] + m1[11] * m2[12],
		m1[8] * m2[1] + m1[9] * m2[5] + m1[10] * m2[9] + m1[11] * m2[13],
		m1[8] * m2[2] + m1[9] * m2[6] + m1[10] * m2[10] + m1[11] * m2[14],
		m1[8] * m2[3] + m1[9] * m2[7] + m1[10] * m2[11] + m1[11] * m2[15],
		m1[12] * m2[0] + m1[13] * m2[4] + m1[14] * m2[8] + m1[15] * m2[12],
		m1[12] * m2[1] + m1[13] * m2[5] + m1[14] * m2[9] + m1[15] * m2[13],
		m1[12] * m2[2] + m1[13] * m2[6] + m1[14] * m2[10] + m1[15] * m2[14],
		m1[12] * m2[3] + m1[13] * m2[7] + m1[14] * m2[11] + m1[15] * m2[15]
	};
}

inline constexpr void Matrix4Old::Set(const float* f)
{
	m[0] = f[0];  m[1] = f[1];  m[2] = f[2];  m[3] = f[3];
	m[4] = f[4];  m[5] = f[5];  m[6] = f[6];  m[7] = f[7];
	m[8] = f[8];  m[9] = f[9];  m[10] = f[10]; m[11] = f[11];
	m[12] = f[12]; m[13] = f[13]; m[14] = f[14]; m[15] = f[15];
}

inline constexpr void Matrix4Old::Set(
	float  m0, float  m1, float  m2, float  m3,
	float  m4, float  m5, float  m6, float  m7,
	float  m8, float  m9, float m10, float m11,
	float m12, float m13, float m14, float m15)
{
	m[0] = m0;  m[1] = m1;  m[2] = m2;  m[3] = m3;
	m[4] = m4;  m[5] = m5;  m[6] = m6;  m[7] = m7;
	m[8] = m8;  m[9] = m9;  m[10] = m10; m[11] = m11;
	m[12] = m12; m[13] = m13; m[14] = m14; m[15] = m15;
}

inline constexpr void Matrix4Old::Set(const Matrix4Old& M)
{
	m[0] = M[0];  m[1] = M[1];   m[2] = M[2];  m[3] = M[3];
	m[4] = M[4];  m[5] = M[5];   m[6] = M[6];  m[7] = M[7];
	m[8] = M[8];  m[9] = M[9];   m[10] = M[10]; m[11] = M[11];
	m[12] = M[12]; m[13] = M[13]; m[14] = M[14]; m[15] = M[15];
}

inline void Matrix4Old::SetTranslation(const Vector3Old & translation)
{
	m[3] = translation.x;
	m[7] = translation.y;
	m[11] = translation.z;
}

inline void Matrix4Old::SetRotation(const Matrix3Old& rotation)
{
	m[0] = rotation[0]; m[1] = rotation[1]; m[2] = rotation[2];
	m[4] = rotation[3]; m[5] = rotation[4]; m[6] = rotation[5];
	m[8] = rotation[6]; m[9] = rotation[7]; m[10] = rotation[8];
}

inline void Matrix4Old::SetScale(const Vector3Old& scale)
{
	m[0] = scale.x;
	m[5] = scale.y;
	m[10] = scale.z;
}

inline void Matrix4Old::SetScale(float scale)
{
	m[0] = scale;
	m[5] = scale;
	m[10] = scale;
}

inline Matrix3Old Matrix4Old::ToMatrix3() const
{
	return {
		m[0], m[1], m[2],
		m[4], m[5], m[6],
		m[8], m[9], m[10]
	};
}

inline Matrix3Old Matrix4Old::RotationMatrix() const
{
	Vector3Old invScale(
		1.0f / sqrtf(m[0] * m[0] + m[4] * m[4] + m[8] * m[8]),
		1.0f / sqrtf(m[1] * m[1] + m[5] * m[5] + m[9] * m[9]),
		1.0f / sqrtf(m[2] * m[2] + m[6] * m[6] + m[10] * m[10])
	);
	return ToMatrix3().Scaled(invScale);
}

inline Vector3Old Matrix4Old::GetTranslation() const
{
	return { m[3], m[7], m[11] };
}

inline QuaternionOld Matrix4Old::GetRotation() const
{
	return QuaternionOld(RotationMatrix());
}

inline Vector3Old Matrix4Old::GetScale() const
{
	return {
		sqrtf(m[0] * m[0] + m[4] * m[4] + m[8] * m[8]),
		sqrtf(m[1] * m[1] + m[5] * m[5] + m[9] * m[9]),
		sqrtf(m[2] * m[2] + m[6] * m[6] + m[10] * m[10])
	};
}

inline Matrix4Old Matrix4Old::Inverse() const
{
	float v0 = m[8] * m[13] - m[9] * m[12];
	float v1 = m[8] * m[14] - m[10] * m[12];
	float v2 = m[8] * m[15] - m[11] * m[12];
	float v3 = m[9] * m[14] - m[10] * m[13];
	float v4 = m[9] * m[15] - m[11] * m[13];
	float v5 = m[10] * m[15] - m[11] * m[14];

	float i00 = (v5 * m[5] - v4 * m[6] + v3 * m[7]);
	float i10 = -(v5 * m[4] - v2 * m[6] + v1 * m[7]);
	float i20 = (v4 * m[4] - v2 * m[5] + v0 * m[7]);
	float i30 = -(v3 * m[4] - v1 * m[5] + v0 * m[6]);

	float invDet = 1.0f / (i00 * m[0] + i10 * m[1] + i20 * m[2] + i30 * m[3]);

	i00 *= invDet;
	i10 *= invDet;
	i20 *= invDet;
	i30 *= invDet;

	float i01 = -(v5 * m[1] - v4 * m[2] + v3 * m[3]) * invDet;
	float i11 = (v5 * m[0] - v2 * m[2] + v1 * m[3]) * invDet;
	float i21 = -(v4 * m[0] - v2 * m[1] + v0 * m[3]) * invDet;
	float i31 = (v3 * m[0] - v1 * m[1] + v0 * m[2]) * invDet;

	v0 = m[4] * m[13] - m[5] * m[12];
	v1 = m[4] * m[14] - m[6] * m[12];
	v2 = m[4] * m[15] - m[7] * m[12];
	v3 = m[5] * m[14] - m[6] * m[13];
	v4 = m[5] * m[15] - m[7] * m[13];
	v5 = m[6] * m[15] - m[7] * m[14];

	float i02 = (v5 * m[1] - v4 * m[2] + v3 * m[3]) * invDet;
	float i12 = -(v5 * m[0] - v2 * m[2] + v1 * m[3]) * invDet;
	float i22 = (v4 * m[0] - v2 * m[1] + v0 * m[3]) * invDet;
	float i32 = -(v3 * m[0] - v1 * m[1] + v0 * m[2]) * invDet;

	v0 = m[9] * m[4] - m[8] * m[5];
	v1 = m[10] * m[4] - m[8] * m[6];
	v2 = m[11] * m[4] - m[8] * m[7];
	v3 = m[10] * m[5] - m[9] * m[6];
	v4 = m[11] * m[5] - m[9] * m[7];
	v5 = m[11] * m[6] - m[10] * m[7];

	float i03 = -(v5 * m[1] - v4 * m[2] + v3 * m[3]) * invDet;
	float i13 = (v5 * m[0] - v2 * m[2] + v1 * m[3]) * invDet;
	float i23 = -(v4 * m[0] - v2 * m[1] + v0 * m[3]) * invDet;
	float i33 = (v3 * m[0] - v1 * m[1] + v0 * m[2]) * invDet;

	return Matrix4Old(
		i00, i01, i02, i03,
		i10, i11, i12, i13,
		i20, i21, i22, i23,
		i30, i31, i32, i33
	);
}

inline Matrix4Old Matrix4Old::Transpose() const
{
	return {
		m[0], m[4], m[ 8], m[12],
		m[1], m[5], m[ 9], m[13],
		m[2], m[6], m[10], m[14],
		m[3], m[7], m[11], m[15]
	};
}

inline void Matrix4Old::Decompose(Vector3Old& translation, QuaternionOld& rotation, Vector3Old& scale) const
{
	translation = GetTranslation();
	scale = GetScale();

	Vector3Old invScale(1.0f / scale.x, 1.0f / scale.y, 1.0f / scale.z);
	rotation = QuaternionOld(ToMatrix3().Scaled(invScale));
}
















inline Matrix4Old Matrix4Old::Rotate(const Vector3Old& axis, float angle)
{
	const float s = sinf(angle);
	const float c = 1.0f - cosf(angle);
	Vector3Old v = axis.GetNormalize();
	const float xx = v.x * v.x;
	const float xy = v.x * v.y;
	const float xz = v.z * v.x;
	const float yy = v.y * v.y;
	const float yz = v.y * v.z;
	const float zz = v.z * v.z;
	const float xs = v.x * s;
	const float ys = v.y * s;
	const float zs = v.z * s;
	Matrix4Old mat;
	mat[0] = 1.0f + c * (xx - 1.0f); mat[4] = c * xy - zs;            mat[8] = c * xz + ys;            mat[12] = 0.0f;
	mat[1] = c * xy + zs;            mat[5] = 1.0f + c * (yy - 1.0f); mat[9] = c * yz - xs;            mat[13] = 0.0f;
	mat[2] = c * xz - ys;            mat[6] = c * yz + xs;            mat[10] = 1.0f + c * (zz - 1.0f); mat[14] = 0.0f;
	mat[3] = 0.0f;                   mat[7] = 0.0f;                   mat[11] = 0.0f;                   mat[15] = 1.0f;
	return mat;
}

inline Matrix4Old Matrix4Old::RotateX(float angle)
{
	const float s = sinf(angle);
	const float c = cosf(angle);

	Matrix4Old mat;
	mat[0] = 1.0f; mat[4] = 0.0f; mat[8] = 0.0f; mat[12] = 0.0f;
	mat[1] = 0.0f; mat[5] = c;    mat[9] = -s;   mat[13] = 0.0f;
	mat[2] = 0.0f; mat[6] = s;    mat[10] = c;    mat[14] = 0.0f;
	mat[3] = 0.0f; mat[7] = 0.0f; mat[11] = 0.0f; mat[15] = 1.0f;
	return mat;
}

inline Matrix4Old Matrix4Old::RotateY(float angle)
{
	const float s = sinf(angle);
	const float c = cosf(angle);

	Matrix4Old mat;
	mat[0] = c;    mat[4] = 0.0f; mat[8] = s;    mat[12] = 0.0f;
	mat[1] = 0.0f; mat[5] = 1.0f; mat[9] = 0.0f; mat[13] = 0.0f;
	mat[2] = -s;   mat[6] = 0.0f; mat[10] = c;    mat[14] = 0.0f;
	mat[3] = 0.0f; mat[7] = 0.0f; mat[11] = 0.0f; mat[15] = 1.0f;
	return mat;
}

inline Matrix4Old Matrix4Old::RotateZ(float angle)
{
	const float s = sinf(angle);
	const float c = cosf(angle);

	Matrix4Old mat;
	mat[0] = c;    mat[4] = -s;   mat[8] = 0.0f; mat[12] = 0.0f;
	mat[1] = s;    mat[5] = c;    mat[9] = 0.0f; mat[13] = 0.0f;
	mat[2] = 0.0f; mat[6] = 0.0f; mat[10] = 1.0f; mat[14] = 0.0f;
	mat[3] = 0.0f; mat[7] = 0.0f; mat[11] = 0.0f; mat[15] = 1.0f;
	return mat;
}

inline Matrix4Old Matrix4Old::Scale(const Vector3Old& v)
{
	Matrix4Old mat;
	mat[0] = v.x;  mat[4] = 0.0f; mat[8] = 0.0f; mat[12] = 0.0f;
	mat[1] = 0.0f; mat[5] = v.y;  mat[9] = 0.0f; mat[13] = 0.0f;
	mat[2] = 0.0f; mat[6] = 0.0f; mat[10] = v.z;  mat[14] = 0.0f;
	mat[3] = 0.0f; mat[7] = 0.0f; mat[11] = 0.0f; mat[15] = 1.0f;
	return mat;
}

inline Matrix4Old Matrix4Old::Translate(const Vector3Old& v)
{
	Matrix4Old mat;
	mat[0] = 1.0f; mat[4] = 0.0f; mat[8] = 0.0f; mat[12] = v.x;
	mat[1] = 0.0f; mat[5] = 1.0f; mat[9] = 0.0f; mat[13] = v.y;
	mat[2] = 0.0f; mat[6] = 0.0f; mat[10] = 1.0f; mat[14] = v.z;
	mat[3] = 0.0f; mat[7] = 0.0f; mat[11] = 0.0f; mat[15] = 1.0f;
	return mat;
}

inline Matrix4Old Matrix4Old::Ortho(float left, float right, float bottom, float top, float n, float f)
{
	Matrix4Old mat;
	mat[0] = 2.0f / (right - left); mat[4] = 0.0f;                  mat[8] = 0.0f;            mat[12] = -(right + left) / (right - left);
	mat[1] = 0.0f;                  mat[5] = 2.0f / (top - bottom); mat[9] = 0.0f;            mat[13] = -(top + bottom) / (top - bottom);
	mat[2] = 0.0f;                  mat[6] = 0.0f;                  mat[10] = -2.0f / (f - n); mat[14] = -(f + n) / (f - n);
	mat[3] = 0.0f;                  mat[7] = 0.0f;                  mat[11] = 0.0f;            mat[15] = 1.0f;
	return mat;
}

inline Matrix4Old Matrix4Old::Perspective(float fieldOfView, float aspectRatio, float znear, float zfar)
{
	const float y = tanf(fieldOfView * 0.5f);
	const float x = y * aspectRatio;
	Matrix4Old mat;
	mat[0] = 1.0f / x; mat[4] = 0.0f;     mat[8] = 0.0f;                              mat[12] = 0.0f;
	mat[1] = 0.0f;     mat[5] = 1.0f / y; mat[9] = 0.0f;                              mat[13] = 0.0f;
	mat[2] = 0.0f;     mat[6] = 0.0f;     mat[10] = -(zfar + znear) / (zfar - znear); mat[14] = -(2.0f * zfar * znear) / (zfar - znear);
	mat[3] = 0.0f;     mat[7] = 0.0f;     mat[11] = -1.0f;                            mat[15] = 0.0f;
	return mat;
}

inline Matrix4Old Matrix4Old::LookAt(const Vector3Old& eye, const Vector3Old& dir, const Vector3Old& up)
{
	const Vector3Old z = (eye - dir).GetNormalize();
	const Vector3Old x = CrossProduct(up, z).GetNormalize();
	//const Vector3 y = CrossProduct(z, x).GetNormalize(); // TODO: удалить?
	const Vector3Old y = CrossProduct(z, x);

	Matrix4Old m0;
	m0[0] = x.x; m0[4] = x.y; m0[8] = x.z; m0[12] = -DotProduct(x, eye);
	m0[1] = y.x; m0[5] = y.y; m0[9] = y.z; m0[13] = -DotProduct(y, eye);
	m0[2] = z.x; m0[6] = z.y; m0[10] = z.z; m0[14] = -DotProduct(z, eye);
	m0[3] = 0.0; m0[7] = 0.0; m0[11] = 0.0; m0[15] = 1.0;
	return m0;
}