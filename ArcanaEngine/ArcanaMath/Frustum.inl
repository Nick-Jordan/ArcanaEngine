
namespace Arcana
{
	template<typename T>
	Frustum<T>::Frustum()
	{
	}

	template<typename T>
	Frustum<T>::Frustum(const Matrix4<T>& matrix)
	{
		set(matrix);
	}

	template<typename T>
	Frustum<T>::Frustum(const Frustum<T>& frustum)
	{
		set(frustum);
	}

	template<typename T>
	Frustum<T>::~Frustum()
	{
	}

	template<typename T>
	void Frustum<T>::set(const Matrix4<T>& matrix)
	{
		_matrix = matrix;

		//Left
		_left.set(matrix.at(3, 3) + matrix.at(3, 0), 
			Vector3<T>(
				matrix.at(0, 3) + matrix.at(0, 0),
				matrix.at(1, 3) + matrix.at(1, 0),
				matrix.at(2, 3) + matrix.at(2, 0)
				));

		//Right 
		_right.set(matrix.at(3, 3) - matrix.at(3, 0),
			Vector3<T>(
				matrix.at(0, 3) - matrix.at(0, 0),
				matrix.at(1, 3) - matrix.at(1, 0),
				matrix.at(2, 3) - matrix.at(2, 0)
				));

		//Top 
		_top.set(matrix.at(3, 3) - matrix.at(3, 1),
			Vector3<T>(
				matrix.at(0, 3) - matrix.at(0, 1),
				matrix.at(1, 3) - matrix.at(1, 1),
				matrix.at(2, 3) - matrix.at(2, 1)
				));

		//Bottom
		_bottom.set(matrix.at(3, 3) + matrix.at(3, 1),
			Vector3<T>(
				matrix.at(0, 3) + matrix.at(0, 1),
				matrix.at(1, 3) + matrix.at(1, 1),
				matrix.at(2, 3) + matrix.at(2, 1)
				));

		//Near
		_near.set(matrix.at(3, 2),
			Vector3<T>(
				matrix.at(0, 2),
				matrix.at(1, 2),
				matrix.at(2, 2)
				));

		//Far
		_far.set(matrix.at(3, 3) - matrix.at(3, 2),
			Vector3<T>(
				matrix.at(0, 3) - matrix.at(0, 2),
				matrix.at(1, 3) - matrix.at(1, 2),
				matrix.at(2, 3) - matrix.at(2, 2)
				));
	}

	template<typename T>
	void Frustum<T>::set(const Frustum<T>& frustum)
	{
		_near = frustum._near;
		_far = frustum._far;
		_bottom = frustum._bottom;
		_top = frustum._top;
		_left = frustum._left;
		_right = frustum._right;
		_matrix = frustum._matrix;
	}

	template<typename T>
	const Plane<T>& Frustum<T>::getNear() const
	{
		return _near;
	}

	template<typename T>
	const Plane<T>& Frustum<T>::getFar() const
	{
		return _far;
	}

	template<typename T>
	const Plane<T>& Frustum<T>::getLeft() const
	{
		return _left;
	}

	template<typename T>
	const Plane<T>& Frustum<T>::getRight() const
	{
		return _right;
	}

	template<typename T>
	const Plane<T>& Frustum<T>::getBottom() const
	{
		return _bottom;
	}

	template<typename T>
	const Plane<T>& Frustum<T>::getTop() const
	{
		return _top;
	}

	template<typename T>
	const Matrix4<T>& Frustum<T>::getMatrix() const
	{
		return _matrix;
	}

	template<typename T>
	void Frustum<T>::getPlanes(std::vector<Plane<T>>& planes) const
	{
		planes.push_back(_left);
		planes.push_back(_right);
		planes.push_back(_top);
		planes.push_back(_bottom);
		planes.push_back(_near);
		planes.push_back(_far);
	}

	template<typename T>
	void Frustum<T>::getCorners(std::vector<Vector3<T>>& corners) const
	{
		getNearCorners(corners);
		getFarCorners(corners);
	}

	template<typename T>
	void Frustum<T>::getNearCorners(std::vector<Vector3<T>>& corners) const
	{
		corners.push_back(Vector3<T>::zero());
		corners.push_back(Vector3<T>::zero());
		corners.push_back(Vector3<T>::zero());
		corners.push_back(Vector3<T>::zero());
		Plane<T>::intersection(_near, _left, _top, corners[corners.size() - 4]);
		Plane<T>::intersection(_near, _left, _bottom, corners[corners.size() - 3]);
		Plane<T>::intersection(_near, _right, _bottom, corners[corners.size() - 2]);
		Plane<T>::intersection(_near, _right, _top, corners[corners.size() - 1]);
	}

	template<typename T>
	void Frustum<T>::getFarCorners(std::vector<Vector3<T>>& corners) const
	{
		corners.push_back(Vector3<T>::zero());
		corners.push_back(Vector3<T>::zero());
		corners.push_back(Vector3<T>::zero());
		corners.push_back(Vector3<T>::zero());
		Plane<T>::intersection(_far, _right, _top, corners[corners.size() - 4]);
		Plane<T>::intersection(_far, _right, _bottom, corners[corners.size() - 3]);
		Plane<T>::intersection(_far, _left, _bottom, corners[corners.size() - 2]);
		Plane<T>::intersection(_far, _left, _top, corners[corners.size() - 1]);
	}

	template<typename T>
	Frustum<T>& Frustum<T>::operator=(const Frustum<T>& frustum)
	{
		set(frustum);
		return *this;
	}
}