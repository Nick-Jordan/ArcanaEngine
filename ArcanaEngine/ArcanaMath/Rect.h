#ifndef RECT_H_
#define RECT_H_

#include "Vector2.h"
#include "Vector4.h"

#include <algorithm>
#include <iostream>

namespace Arcana
{
	/** \brief A basic 2D rectangle.
	 *
	 *  Defined with left/top/width/height or point/size.
	 */
	template<typename T>
	class Rect
	{
	public:

		/** \brief Rect default constructor.
		 *
		 *  Creates an empty rectangle.
		 */
		Rect();

		/** \brief Rect left/top/width/height constructor.
		 */
		Rect(T left, T top, T width, T height);

		/** \brief Rect point/size constructor.
		 */
		Rect(const Vector2<T>& point, const Vector2<T>& size);

		/** \brief Rect 4-vector constructor.
		 *
		 *  Defined as left = v.x, top = v.y, width = v.z, height = v.w
		 */
		Rect(const Vector4<T>& extents);

		/** \brief Rect copy constructor.
		 */
		Rect(const Rect<T>& rect);

		/** \brief Rect destructor
		 */
		~Rect();

		/** \brief Rect assignment operator
		 */
		Rect<T>& operator=(const Rect<T>& rect);

		bool operator==(const Rect<T>& rect) const;

		bool operator!=(const Rect<T>& rect) const;

		/** \brief Returns true if this rectangle contains the point.
		 */
		bool contains(T x, T y) const;
		
		/** \brief Returns true if this rectangle contains the point.
		 */
		bool contains(const Vector2<T>& point) const;

		/** \brief Returns true if this rectangle contains the argument rectangle.
		 */
		bool contains(const Rect<T>& rect) const;

		/** \brief Returns the bottom of the rectangle.
		 *
		 *  Defined as bottom = top + height
		 */
		T getBottom() const;

		/** \brief Returns the top of the rectangle.
		 */
		T getTop() const;

		/** \brief Returns the left of the rectangle.
		 */
		T getLeft() const;

		/** \brief Returns the right of the rectangle.
		 *
		 *  Defined as right = left + width
		 */
		T getRight() const;

		/** \brief Returns the size of the rectangle.
		 *
		 *  Defined as v = <width, height>
		 */
		Vector2<T> getSize() const;

		/** \brief Returns the size of the rectangle.
		 *
		 *  Defined as v = <left, top>
		 */
		Vector2<T> getPosition() const;

		/** \brief Returns the bounds of the rectangle.
		 *
		 *  Defined as v = <left, top, width, height>
		 */
		Rect<T> getBounds() const;

		/** \brief Changes the bounds of the rectangle by <dx, dy> in every direction.
		 */
		void resize(T dx, T dy);

		/** \brief Changes the bounds of the rectangle by <ds> in every direction.
		 */
		void resize(const Vector2<T>& ds);

		/** \brief Offsets the rectangle top-left position.
		 */
		void offset(T dx, T dy);

		/** \brief Offsets the rectangle top-left position.
		 */
		void offset(const Vector2<T>& ds);

		/** \brief Sets this rectangle to the intersection with the argument rectangle.
		 */
		void intersect(const Rect<T>& rect);

		/** \brief Sets result to the intersection of the two argument rectangles.
		 */
		void intersect(const Rect<T>& rect1, const Rect<T>& rect2, Rect<T>& result);

		/** \brief Returns true if the two rectangles intersect.
		 */
		bool intersects(const Rect<T>& rect) const;

		/** \brief Returns true if width and height are zero.
		 */
		bool isEmpty() const;

		/** \brief Sets this rectangle to the union with the argument rectangle.
		 */
		void rectUnion(const Rect<T>& rect);

		/** \brief Sets result to the union of the two argument rectangles.
		 */
		void rectUnion(const Rect<T>& rect1, const Rect<T>& rect2, Rect<T>& result);

		/** \brief Casts this rectangle to another type.
		 */
		template<typename N>
		Rect<N> cast()
		{
			return Rect<N>((N)_left, (N)_top, (N)_width, (N) _height);
		}

	private:

		T _left;	///< The x-coordinate of the left of the rectangle.
		T _top;		///< The y-coordinate of the top of the rectangle.
		T _width;	///< The rectangle width.
		T _height;  ///< The rectangle height.
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
	Rect<T>& Rect<T>::operator=(const Rect<T>& rect)
	{
		_left = rect._left;
		_top = rect._top;
		_width = rect._width;
		_height = rect._height;

		return *this;
	}

	template<typename T>
	bool Rect<T>::operator==(const Rect<T>& rect) const
	{
		return _left == rect._left && _top == rect._top
			&& _width == rect._width && _height == rect._height;
	}

	template<typename T>
	bool Rect<T>::operator!=(const Rect<T>& rect) const
	{
		return !(*this == rect);
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
	inline Vector2<T> Rect<T>::getSize() const
	{
		return Vector2<T>(_width, _height);
	}

	template<typename T>
	inline Vector2<T> Rect<T>::getPosition() const
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
		intersect(*this, rect, *this);
	}

	template<typename T>
	inline void Rect<T>::intersect(const Rect<T>& rect1, const Rect<T>& rect2, Rect<T>& result)
	{
		if (!rect1.intersects(rect2))
		{
			result = Rect<T>(0, 0, 0, 0);
			return;
		}

		if (rect1.contains(rect2))
		{
			result = rect2;
			return;
		}

		if (rect2.contains(rect1))
		{
			result = rect1;
			return;
		}

		Vector2<T> position;
		Vector2<T> position2;

		position.x = (std::max)(rect1.getLeft(), rect2.getLeft());
		position.y = (std::max)(rect1.getTop(), rect2.getTop());

		position2.x = (std::min)(rect1.getRight(), rect2.getRight());
		position2.y = (std::min)(rect1.getBottom(), rect2.getBottom());

		result = Rect<T>(position, position2 - position);
	}

	template<typename T>
	inline bool Rect<T>::intersects(const Rect<T>& rect) const
	{
		return _left < rect.getRight() && getRight() > rect._left &&
			_top < rect.getBottom() && getBottom() > rect._top;
	}

	template<typename T>
	inline bool Rect<T>::isEmpty() const
	{
		return _width == (T)0 || _height == (T)0;
	}

	template<typename T>
	inline void Rect<T>::rectUnion(const Rect<T>& rect)
	{
		rectUnion(*this, rect, *this);
	}

	template<typename T>
	inline void Rect<T>::rectUnion(const Rect<T>& rect1, const Rect<T>& rect2, Rect<T>& result)
	{
		if (rect1.contains(rect2))
		{
			result = rect1;
			return;
		}
		if (rect2.contains(rect1))
		{
			result = rect2;
			return;
		}

		Vector2<T> position;
		Vector2<T> position2;

		position.x = (std::min)(rect1.getLeft(), rect2.getLeft());
		position.y = (std::min)(rect1.getTop(), rect2.getTop());

		position2.x = (std::max)(rect1.getRight(), rect2.getRight());
		position2.y = (std::max)(rect1.getBottom(), rect2.getBottom());

		result = Rect<T>(position, position2 - position);
	}


	typedef Rect<float> Rectf;
	typedef Rect<double> Rectd;
	typedef Rect<int> Recti;
}

#endif // !RECT_H_
