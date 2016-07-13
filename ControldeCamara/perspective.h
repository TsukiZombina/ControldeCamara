#pragma once
#ifndef PERSPECTIVE_H
#define PERSPECTIVE_H

#include "squarematrix.h"
#include "vector3D.h"

	template<class T, size_t order>
	SquareMatrix<T, order>& MakePerspective(GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far, SquareMatrix<T, order>& perspective);
	template<class T, size_t order>
	SquareMatrix<T, order>& MakeView(Vector3D<T>& position, Vector3D<T>& target, Vector3D<T>& up, SquareMatrix<T, order>& view);


	template<class T, size_t order>
	SquareMatrix<T, order>& MakePerspective(GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far, SquareMatrix<T, order>& perspective)
	{
		perspective.InitZero();

		// Convert fov to radians
		fov = fov * 3.141592654f / 180;

		perspective.set(0, 0, (1 / aspect) * (1 / tanf(fov / 2)));
		perspective.set(1, 1, 1 / tanf(fov / 2));
		perspective.set(2, 2, -(far + near) / (far - near));
		perspective.set(2, 3, -(2 * far * near) / (far - near));
		perspective.set(3, 2, -1);

		return perspective;
	}

	template<class T, size_t order>
	SquareMatrix<T, order>& MakeView(Vector3D<T>& position, Vector3D<T>& target, Vector3D<T>& up, SquareMatrix<T, order>& view)
	{
		Vector3D<float> f(target(0) - position(0), target(1) - position(1), target(2) - position(2));
		f = f.Normalize();

		Vector3D<float> s;
		s = CrossNormalize(f, up);

		Vector3D<float> v;
		v = CrossNormalize(s, f);

		view.Identity();

		view.set(0, 0, s(0));
		view.set(0, 1, s(1));
		view.set(0, 2, s(2));

		view.set(1, 0, v(0));
		view.set(1, 1, v(1));
		view.set(1, 2, v(2));

		view.set(2, 0, -f(0));
		view.set(2, 1, -f(1));
		view.set(2, 2, -f(2));

		view.set(0, 3, -Dot(s, position));
		view.set(1, 3, -Dot(v, position));
		view.set(2, 3, Dot(f, position));

		return view;
	}

#endif //PERSPECTIVE_H