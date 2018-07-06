#ifndef RECT_H_
#define RECT_H_

#include "Vector2.h"
#include "Vector4.h"

namespace Arcana
{

	template<typename T>
	class Rect
	{
	public:

		Rect();

		Rect(T left, T top, T width, T height);

		Rect(const Vector2<T>& point, const Vector2<T>& size);

		Rect(const Vector4<T>& extents);

		Rect(const Rect<T>& rect);

		~Rect();


		bool contains(T x, T y) const;
		
		bool contains(const Vector2<T>& point) const;

		bool contains(const Rect<T>& rect) const;

		T getBottom() const;

		T getTop() const;

		T getLeft() const;

		T getRight() const;

		const Vector2<T>& getSize() const;

		const Vector2<T>& getPosition() const;

		Rect<T> getBounds() const;

		void resize(T dx, T dy);

		void resize(const Vector2<T>& ds);

		void offset(T dx, T dy);

		void offset(const Vector2<T>& ds);

		void intersect(const Rect<T>& rect);

		void intersect(const Rect<T>& rect1, const Rect<T>& rect2, const Rect<T>& result);

		bool intersects(const Rect<T>& rect);

		bool isEmpty() const;

		void rectUnion(const Rect<T>& rect);

		void rectUnion(const Rect<T>& rect1, const Rect<T>& rect2, const Rect<T>& result);

		template<typename N>
		Rect<N> cast()
		{
			return Rect<N>((N)_left, (N)_top, (N)_width, (N) _height);
		}

	private:

		T _left;
		T _top;
		T _width;
		T _height;
	};

	template<typename T>
	inline Rect<T>::Rect() : _left((T)0), _top((T)0), _width((T)0), _height((T)0)
	{

	}

	template<typename T>
	inline Rect<T>::Rect(T left, T top, T width, T height) : _left(left), _top(top), _width(width), _height(height)
	{

	}

	template<typename T>
	inline Rect<T>::Rect(const Vector2<T>& point, const Vector2<T>& size) : _left(point.x), _top(point.y), _width(size.x), _height(size.y)
	{

	}

	template<typename T>
	inline Rect<T>::Rect(const Vector4<T>& extents) : _left(extents.x), _top(extents.y), _width(extents.z), _height(extents.w)
	{

	}

	template<typename T>
	inline Rect<T>::Rect(const Rect<T>& rect) : _left(rect._left), _top(rect._top), _width(rect._width), _height(rect._height)
	{

	}

	template<typename T>
	inline Rect<T>::~Rect()
	{

	}

	template<typename T>
	inline bool Rect<T>::contains(T x, T y) const
	{
		return x >= _left && x <= getRight()
			&& y >= _top && y <= getBottom();
	}

	template<typename T>
	inline bool Rect<T>::contains(const Vector2<T>& point) const
	{
		return contains(point.x, point.y);
	}

	template<typename T>
	inline bool Rect<T>::contains(const Rect<T>& rect) const
	{
		return contains(rect.getPosition()) && contains(rect.getPosition() + rect.getSize());
	}

	template<typename T>
	inline T Rect<T>::getBottom() const
	{
		return _top + _height;
	}

	template<typename T>
	inline T Rect<T>::getTop() const
	{
		return _top;
	}

	template<typename T>
	inline T Rect<T>::getLeft() const
	{
		return _left;
	}

	template<typename T>
	inline T Rect<T>::getRight() const
	{
		return _left + _width;
	}

	template<typename T>
	inline const Vector2<T>& Rect<T>::getSize() const
	{
		return Vector2<T>(_width, _height);
	}

	template<typename T>
	inline const Vector2<T>& Rect<T>::getPosition() const
	{
		return Vector2<T>(_left, _top);
	}

	template<typename T>
	inline Rect<T> Rect<T>::getBounds() const
	{
		return Rect<T>(_left, _top, _width, _height);
	}

	template<typename T>
	inline void Rect<T>::resize(T dx, T dy)
	{
		_width += dx;
		_left -= dx;
		_height += dy;
		_top -= dy;
	}

	template<typename T>
	inline void Rect<T>::resize(const Vector2<T>& ds)
	{
		resize(ds.x, ds.y);
	}

	template<typename T>
	inline void Rect<T>::offset(T dx, T dy)
	{
		_left += dx;
		_top += dy;
	}

	template<typename T>
	inline void Rect<T>::offset(const Vector2<T>& ds)
	{
		offset(ds.x, ds.y);
	}

	template<typename T>
	inline void Rect<T>::intersect(const Rect<T>& rect)
	{
		
	}

	template<typename T>
	inline void Rect<T>::intersect(const Rect<T>& rect1, const Rect<T>& rect2, const Rect<T>& result)
	{

	}

	template<typename T>
	inline bool Rect<T>::intersects(const Rect<T>& rect)
	{
		return _left < rect.getRight() && getRight() > rect._left &&
			_top > rect.getBottom() && getBottom() < rect._top;
	}

	template<typename T>
	inline bool Rect<T>::isEmpty() const
	{
		return _width == (T)0 && _height == (T)0;
	}

	template<typename T>
	inline void Rect<T>::rectUnion(const Rect<T>& rect)
	{
		
	}

	template<typename T>
	inline void Rect<T>::rectUnion(const Rect<T>& rect1, const Rect<T>& rect2, const Rect<T>& result)
	{

	}


	typedef Rect<float> Rectf;
	typedef Rect<double> Rectd;
	typedef Rect<int> Recti;
}

#endif // !RECT_H_
