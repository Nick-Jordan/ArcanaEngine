#ifndef FRUSTUM_H_
#define FRUSTUM_H_

#include "Matrix4.h"
#include "Plane.h"

#include <vector>

namespace Arcana
{
	template<typename T>
	class Frustum
	{
	public:

		Frustum();

		Frustum(const Matrix4<T>& matrix);

		Frustum(const Frustum<T>& frustum);

		~Frustum();

		void set(const Matrix4<T>& matrix);

		void set(const Frustum<T>& frustum);

		const Plane<T>& getNear() const;

		const Plane<T>& getFar() const;

		const Plane<T>& getLeft() const;

		const Plane<T>& getRight() const;

		const Plane<T>& getBottom() const;

		const Plane<T>& getTop() const;

		const Matrix4<T>& getMatrix() const;

		void getPlanes(std::vector<Plane<T>>& planes) const;

		void getCorners(std::vector<Vector3<T>>& corners) const;

		void getNearCorners(std::vector<Vector3<T>>& corners) const;

		void getFarCorners(std::vector<Vector3<T>>& corners) const;

		Frustum<T>& operator=(const Frustum<T>& frustum);

	private:

		Plane<T> _near;
		Plane<T> _far;
		Plane<T> _bottom;
		Plane<T> _top;
		Plane<T> _left;
		Plane<T> _right;
		Matrix4<T> _matrix;
	};

	typedef Frustum<float> Frustumf;
	typedef Frustum<double> Frustumd;
	typedef Frustum<int> Frustumi;
}

#include "Frustum.inl"

#endif // !FRUSTUM_H_
