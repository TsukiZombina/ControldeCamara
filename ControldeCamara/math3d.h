#pragma once
#ifndef MATH3D_H
#define MATH3D_H

struct Vector3
{
	float x;
	float y;
	float z;
};

struct Matrix3x3
{
	float m[3][3];
};

struct Matrix4x4
{
	float m[4][4];
};

void Vector3InitToZero(Vector3* vector)
{
	vector->x = 0.0f;
	vector->y = 0.0f;
	vector->z = 0.0f;
}

Vector3 operator+(const Vector3& left, const Vector3& right)
{
	Vector3 result;

	result.x = left.x + right.x;
	result.y = left.y + right.y;
	result.z = left.z + right.z;

	return result;
}

Vector3 operator-(const Vector3& left, const Vector3& right)
{
	Vector3 result;

	result.x = left.x - right.x;
	result.y = left.y - right.y;
	result.z = left.z - right.z;

	return result;
}

Vector3 operator*(float factor, const Vector3& vector)
{
	Vector3 result;

	result.x = factor * vector.x;
	result.y = factor * vector.y;
	result.z = factor * vector.z;

	return result;
}

void Vector3Normalize(Vector3* vector)
{
	float length = sqrtf(vector->x * vector->x +
		vector->y * vector->y +
		vector->z * vector->z);

	vector->x /= length;
	vector->y /= length;
	vector->z /= length;
}

float Vector3ScalarProduct(Vector3* v1, Vector3* v2)
{
	float result = v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;

	return result;
}

void Vector3CrossProduct(Vector3* v1, Vector3* v2, Vector3* result)
{
	result->x = v1->y * v2->z - v1->z * v2->y;
	result->y = v1->z * v2->x - v1->x * v2->z;
	result->z = v1->x * v2->y - v1->y * v2->x;
}

void Matrix3x3InitToZero(Matrix3x3* matrix)
{
	matrix->m[0][0] = 0.0f;    matrix->m[0][1] = 0.0f;    matrix->m[0][2] = 0.0f;
	matrix->m[1][0] = 0.0f;    matrix->m[1][1] = 0.0f;    matrix->m[1][2] = 0.0f;
	matrix->m[2][0] = 0.0f;    matrix->m[2][1] = 0.0f;    matrix->m[2][2] = 0.0f;
}

void Matrix3x3MakeIdentity(Matrix3x3* matrix)
{
	matrix->m[0][0] = 1.0f;    matrix->m[0][1] = 0.0f;    matrix->m[0][2] = 0.0f;
	matrix->m[1][0] = 0.0f;    matrix->m[1][1] = 1.0f;    matrix->m[1][2] = 0.0f;
	matrix->m[2][0] = 0.0f;    matrix->m[2][1] = 0.0f;    matrix->m[2][2] = 1.0f;
}

void Matrix3x3Multiplication(Matrix3x3* op1, Matrix3x3* op2, Matrix3x3* result)
{
	for (unsigned int row = 0; row < 3; row++)
	{
		for (unsigned int col = 0; col < 3; col++)
		{
			result->m[row][col] = op1->m[row][0] * op2->m[0][col] +
				op1->m[row][1] * op2->m[1][col] +
				op1->m[row][2] * op2->m[2][col];
		}
	}
}

void Matrix3x3MultiplicationByVector(Matrix3x3* matrix, Vector3* vector, Vector3* result)
{
	result->x = matrix->m[0][0] * vector->x +
		matrix->m[0][1] * vector->y +
		matrix->m[0][2] * vector->z;

	result->y = matrix->m[1][0] * vector->x +
		matrix->m[1][1] * vector->y +
		matrix->m[1][2] * vector->z;

	result->z = matrix->m[2][0] * vector->x +
		matrix->m[2][1] * vector->y +
		matrix->m[2][2] * vector->z;
}

void Matrix3x3MakeRotation(Vector3* axis, float angle, Matrix3x3* result)
{
	// Convert angle to radians
	angle = angle * 3.141592654f / 180;

	float cosAngle = cosf(angle);
	float sinAngle = sinf(angle);

	result->m[0][0] = cosAngle + (1 - cosAngle) * axis->x * axis->x;
	result->m[0][1] = (1 - cosAngle) * axis->x * axis->y - sinAngle * axis->z;
	result->m[0][2] = (1 - cosAngle) * axis->x * axis->z + sinAngle * axis->y;

	result->m[1][0] = (1 - cosAngle) * axis->x * axis->x + sinAngle * axis->z;
	result->m[1][1] = cosAngle + (1 - cosAngle) * axis->y * axis->y;
	result->m[1][2] = (1 - cosAngle) * axis->y * axis->z - sinAngle * axis->x;

	result->m[2][0] = (1 - cosAngle) * axis->x * axis->z - sinAngle * axis->y;
	result->m[2][1] = (1 - cosAngle) * axis->y * axis->z + sinAngle * axis->x;
	result->m[2][2] = cosAngle + (1 - cosAngle) * axis->z * axis->z;
}

void Matrix3x3MakeRotationY(float angle, Matrix3x3* result)
{
	// Convert angle to radians
	angle = angle * 3.141592654f / 180;

	Matrix3x3MakeIdentity(result);

	float cosAngle = cosf(angle);
	float sinAngle = sinf(angle);

	result->m[0][0] = cosAngle;
	result->m[0][2] = sinAngle;
	result->m[2][0] = -sinAngle;
	result->m[2][2] = cosAngle;
}

void Matrix4x4InitToZero(Matrix4x4* matrix)
{
	matrix->m[0][0] = 0.0f;    matrix->m[0][1] = 0.0f;    matrix->m[0][2] = 0.0f;    matrix->m[0][3] = 0.0f;
	matrix->m[1][0] = 0.0f;    matrix->m[1][1] = 0.0f;    matrix->m[1][2] = 0.0f;    matrix->m[1][3] = 0.0f;
	matrix->m[2][0] = 0.0f;    matrix->m[2][1] = 0.0f;    matrix->m[2][2] = 0.0f;    matrix->m[2][3] = 0.0f;
	matrix->m[3][0] = 0.0f;    matrix->m[3][1] = 0.0f;    matrix->m[3][2] = 0.0f;    matrix->m[3][3] = 0.0f;
}

void Matrix4x4MakeIdentity(Matrix4x4* matrix)
{
	matrix->m[0][0] = 1.0f;    matrix->m[0][1] = 0.0f;    matrix->m[0][2] = 0.0f;    matrix->m[0][3] = 0.0f;
	matrix->m[1][0] = 0.0f;    matrix->m[1][1] = 1.0f;    matrix->m[1][2] = 0.0f;    matrix->m[1][3] = 0.0f;
	matrix->m[2][0] = 0.0f;    matrix->m[2][1] = 0.0f;    matrix->m[2][2] = 1.0f;    matrix->m[2][3] = 0.0f;
	matrix->m[3][0] = 0.0f;    matrix->m[3][1] = 0.0f;    matrix->m[3][2] = 0.0f;    matrix->m[3][3] = 1.0f;
}

void Matrix4x4Multiplication(Matrix4x4* op1, Matrix4x4* op2, Matrix4x4* result)
{
	for (unsigned int row = 0; row < 4; row++)
	{
		for (unsigned int col = 0; col < 4; col++)
		{
			result->m[row][col] = op1->m[row][0] * op2->m[0][col] +
				op1->m[row][1] * op2->m[1][col] +
				op1->m[row][2] * op2->m[2][col] +
				op1->m[row][3] * op2->m[3][col];
		}
	}
}

Matrix4x4 operator*(const Matrix4x4& op1, Matrix4x4& op2)
{
	Matrix4x4 result;

	for (unsigned int row = 0; row < 4; row++)
	{
		for (unsigned int col = 0; col < 4; col++)
		{
			result.m[row][col] = op1.m[row][0] * op2.m[0][col] +
				op1.m[row][1] * op2.m[1][col] +
				op1.m[row][2] * op2.m[2][col] +
				op1.m[row][3] * op2.m[3][col];
		}
	}

	return result;
}

void Matrix4x4MakeScaling(float scaleX, float scaleY, float scaleZ, Matrix4x4* result)
{
	Matrix4x4MakeIdentity(result);

	result->m[0][0] = scaleX;
	result->m[1][1] = scaleY;
	result->m[2][2] = scaleZ;
}

void Matrix4x4MakeUniformScaling(float scale, Matrix4x4* result)
{
	Matrix4x4MakeIdentity(result);

	result->m[0][0] = scale;
	result->m[1][1] = scale;
	result->m[2][2] = scale;
}

void Matrix4x4MakeTraslation(float offsetX, float offsetY, float offsetZ, Matrix4x4* result)
{
	Matrix4x4MakeIdentity(result);

	result->m[0][3] = offsetX;
	result->m[1][3] = offsetY;
	result->m[2][3] = offsetZ;
}

void Matrix4x4MakeRotationY(float angle, Matrix4x4* result)
{
	// Convert angle to radians
	angle = angle * 3.141592654f / 180;

	Matrix4x4MakeIdentity(result);

	float cosAngle = cosf(angle);
	float sinAngle = sinf(angle);

	result->m[0][0] = cosAngle;
	result->m[0][2] = sinAngle;
	result->m[2][0] = -sinAngle;
	result->m[2][2] = cosAngle;
}

void Matrix4x4MakeRotationZ(float angle, Matrix4x4* result)
{
	// Convert angle to radians
	angle = angle * 3.141592654f / 180;

	Matrix4x4MakeIdentity(result);

	float cosAngle = cosf(angle);
	float sinAngle = sinf(angle);

	result->m[0][0] = cosAngle;
	result->m[0][1] = -sinAngle;
	result->m[1][0] = sinAngle;
	result->m[1][1] = cosAngle;
}

void Matrix4x4MakeView(Vector3* position, Vector3* target, Vector3* up, Matrix4x4* view)
{
	Vector3 f;
	f.x = target->x - position->x;
	f.y = target->y - position->y;
	f.z = target->z - position->z;
	Vector3Normalize(&f);

	Vector3 s;
	Vector3CrossProduct(&f, up, &s);
	Vector3Normalize(&s);

	Vector3 v;
	Vector3CrossProduct(&s, &f, &v);

	Matrix4x4MakeIdentity(view);

	view->m[0][0] = s.x;
	view->m[0][1] = s.y;
	view->m[0][2] = s.z;

	view->m[1][0] = v.x;
	view->m[1][1] = v.y;
	view->m[1][2] = v.z;

	view->m[2][0] = -f.x;
	view->m[2][1] = -f.y;
	view->m[2][2] = -f.z;

	view->m[0][3] = -Vector3ScalarProduct(&s, position);
	view->m[1][3] = -Vector3ScalarProduct(&v, position);
	view->m[2][3] = Vector3ScalarProduct(&f, position);
}

void Matrix4x4MakePerspective(GLfloat fov, GLfloat aspect, GLfloat near,
	GLfloat far, Matrix4x4* perspective)
{
	Matrix4x4InitToZero(perspective);

	// Convert fov to radians
	fov = fov * 3.141592654f / 180;

	// glm, correct
	perspective->m[0][0] = (1 / aspect) * (1 / tanf(fov / 2));
	perspective->m[1][1] = 1 / tanf(fov / 2);
	perspective->m[2][2] = -(far + near) / (far - near);
	perspective->m[2][3] = -(2 * far * near) / (far - near);
	perspective->m[3][2] = -1;
}

void Matrix4x4MakePerspectiveSimple(GLfloat left, GLfloat right, GLfloat bottom,
	GLfloat top, GLfloat near, GLfloat far, Matrix4x4* perspective)
{
	Matrix4x4InitToZero(perspective);

	perspective->m[0][0] = (2 * near) / (right - left);
	perspective->m[0][2] = (right + left) / (right - left);
	perspective->m[1][1] = (2 * near) / (top - bottom);
	perspective->m[1][2] = (top + bottom) / (top - bottom);
	perspective->m[2][2] = -(far + near) / (far - near);
	perspective->m[2][3] = -(2 * far * near) / (far - near);
	perspective->m[3][2] = -1;
}

#endif // MATH3D_H
