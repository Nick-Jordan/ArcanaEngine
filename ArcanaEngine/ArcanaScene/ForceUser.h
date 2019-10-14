#ifndef FORCE_USER_H_
#define FORCE_USER_H_

#include "Vector3.h"
#include "Array.h"

namespace Arcana
{
	template<typename T>
	class Force
	{
	public:

		virtual ~Force() {};

		virtual Vector3<T> apply(const Vector3<T>& position, const Vector3<T>& velocity, const Vector3<T>& acceleration, const T& mass) = 0;
	};

	//test/////////
	template<typename T>
	class ConstantAccelerationForce : public Force<T>
	{
	public:

		ConstantAccelerationForce(const Vector3<T>& acceleration) : _acceleration(acceleration)
		{

		}

		virtual Vector3<T> apply(const Vector3<T>& position, const Vector3<T>& velocity, const Vector3<T>& acceleration, const T& mass) override
		{
			return _acceleration / mass;
		}

	private:

		Vector3<T> _acceleration;
	};
	template<typename T>
	class DragForce : public Force<T>
	{
	public:

		DragForce(const T& dragConstant) : _dragConstant(dragConstant)
		{

		}

		virtual Vector3<T> apply(const Vector3<T>& position, const Vector3<T>& velocity, const Vector3<T>& acceleration, const T& mass) override
		{
			return -_dragConstant * velocity;
		}

	private:

		T _dragConstant;
	};
	template<typename T>
	class SpringForce : public Force<T>
	{
	public:

		SpringForce(const Vector3<T>& position, const T& k, const T& restLength) : _position(position), _k(k), _length(restLength)
		{

		}

		virtual Vector3<T> apply(const Vector3<T>& position, const Vector3<T>& velocity, const Vector3<T>& acceleration, const T& mass) override
		{
			Vector3<T> d = position - _position;
			T dl = d.magnitude();
			d.normalize();

			return -_k * (dl - _length) * d;
		}

	private:

		Vector3<T> _position;
		T _k;
		T _length;
	};
	//test/////////////
	
	//move implementations to inl file
	template<typename T>
	class ForceUser
	{
	public:

		virtual ~ForceUser()
		{
			for (auto i = _forces.createIterator(); i; i++)
			{
				AE_DELETE(*i);
			}
		}

		void addForce(Force<T>* force)
		{
			if (force)
			{
				_forces.add(force);
			}
		}

		void removeForce(Force<T>* force)
		{
			if (force)
			{
				_forces.remove(force);
			}
		}

		Vector3<T> apply(const Vector3<T>& position, const Vector3<T>& velocity, const Vector3<T>& acceleration, const T& mass)
		{
			Vector3<T> deltaAcceleration;
			for (auto i = _forces.createConstIterator(); i; i++)
			{
				Force<T>* force = *i;
				if (force)
				{
					deltaAcceleration += force->apply(position, velocity, acceleration, mass);
				}
			}
			return deltaAcceleration;
		}

	private:

		Array<Force<T>*> _forces;
	};
}

#endif // !FORCE_USER_H_

