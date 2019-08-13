#ifndef UNION_H_
#define UNION_H_

#include "Types.h"

namespace Arcana
{
	struct EmptyType
	{
		bool operator==(const EmptyType& ) const
		{
			return true;
		}

		bool operator!=(const EmptyType&) const
		{
			return false;
		}
	};

	template<
		typename SubtypeA, 
		typename SubtypeB = EmptyType, typename SubtypeC = EmptyType, 
		typename SubtypeD = EmptyType, typename SubtypeE = EmptyType, 
		typename SubtypeF = EmptyType, typename SubtypeG = EmptyType, 
		typename SubtypeH = EmptyType, typename SubtypeI = EmptyType, 
		typename SubtypeJ = EmptyType, typename SubtypeK = EmptyType, 
		typename SubtypeL = EmptyType, typename SubtypeM = EmptyType, 
		typename SubtypeN = EmptyType, typename SubtypeO = EmptyType, 
		typename SubtypeP = EmptyType
	>
	class Union
	{
	public:

		Union() : _currentIndex(-1)
		{

		}

		Union(const Union& copy) : _currentIndex(-1)
		{
			switch (copy._currentIndex)
			{
			case -1: 
				break;
			case 0: 
				set<SubtypeA>(copy.get<SubtypeA>());
				break;
			case 1:
				set<SubtypeB>(copy.get<SubtypeB>());
				break;
			case 2:
				set<SubtypeC>(copy.get<SubtypeC>());
				break;
			case 3:
				set<SubtypeD>(copy.get<SubtypeD>());
				break;
			case 4:
				set<SubtypeE>(copy.get<SubtypeE>());
				break;
			case 5:
				set<SubtypeF>(copy.get<SubtypeF>());
				break;
			case 6:
				set<SubtypeG>(copy.get<SubtypeG>());
				break;
			case 7:
				set<SubtypeH>(copy.get<SubtypeH>());
				break;
			case 8:
				set<SubtypeI>(copy.get<SubtypeI>());
				break;
			case 9:
				set<SubtypeJ>(copy.get<SubtypeJ>());
				break;
			case 10:
				set<SubtypeK>(copy.get<SubtypeK>());
				break;
			case 11:
				set<SubtypeL>(copy.get<SubtypeL>());
				break;
			case 12:
				set<SubtypeM>(copy.get<SubtypeM>());
				break;
			case 13:
				set<SubtypeN>(copy.get<SubtypeN>());
				break;
			case 14:
				set<SubtypeO>(copy.get<SubtypeO>());
				break;
			case 15:
				set<SubtypeP>(copy.get<SubtypeP>());
				break;
			default:
				break;
			};
		}

		~Union()
		{
			reset();
		}

		void reset()
		{
			switch (_currentIndex)
			{
			case -1:
				break;
			case 0:
				resetType<SubtypeA>();
				break;
			case 1:
				resetType<SubtypeB>();
				break;
			case 2:
				resetType<SubtypeC>();
				break;
			case 3:
				resetType<SubtypeD>();
				break;
			case 4:
				resetType<SubtypeE>();
				break;
			case 5:
				resetType<SubtypeF>();
				break;
			case 6:
				resetType<SubtypeG>();
				break;
			case 7:
				resetType<SubtypeH>();
				break;
			case 8:
				resetType<SubtypeI>();
				break;
			case 9:
				resetType<SubtypeJ>();
				break;
			case 10:
				resetType<SubtypeK>();
				break;
			case 11:
				resetType<SubtypeL>();
				break;
			case 12:
				resetType<SubtypeM>();
				break;
			case 13:
				resetType<SubtypeN>();
				break;
			case 14:
				resetType<SubtypeO>();
				break;
			case 15:
				resetType<SubtypeP>();
				break;
			default:
				break;
			};
		}

		template<typename Subtype>
		Subtype* set(const Subtype& value)
		{
			int32 index;
			Subtype* type;
			get<Subtype, Subtype*>(*this, index, type);

			reset();

			new(type) Subtype(value);

			_currentIndex = index;

			return type;
		}

		template<typename Subtype>
		const Subtype& get() const
		{
			int32 index;
			const Subtype* type;
			get<Subtype, const Subtype*>(*this, index, type);

			AE_ASSERT(_currentIndex == index);

			return *type;
		}

		template<typename Subtype>
		Subtype& get()
		{
			int32 index;
			Subtype* type;
			get<Subtype, Subtype*>(*this, index, type);

			AE_ASSERT(_currentIndex == index);

			return *type;
		}

		template<typename Subtype>
		bool isType() const
		{
			int32 index;
			const Subtype* type;
			get<Subtype, const Subtype*>(*this, index, type);
			return _currentIndex == index;
		}

		template<typename Subtype>
		void resetType()
		{
			int32 index;
			Subtype* type;
			get<Subtype, Subtype*>(*this, index, type);

			if (_currentIndex == index)
			{
				_currentIndex = -1;

				type->~Subtype();
			}
		}

		bool operator==(const Union& rhs) const
		{
			if (_currentIndex == rhs._currentIndex)
			{
				switch (_currentIndex)
				{
				case 0: 
					return get<SubtypeA>() == rhs.get<SubtypeA>();
				case 1:
					return get<SubtypeB>() == rhs.get<SubtypeB>();
				case 2:
					return get<SubtypeC>() == rhs.get<SubtypeC>();
				case 3:
					return get<SubtypeD>() == rhs.get<SubtypeD>();
				case 4:
					return get<SubtypeE>() == rhs.get<SubtypeE>();
				case 5:
					return get<SubtypeF>() == rhs.get<SubtypeF>();
				case 6:
					return get<SubtypeG>() == rhs.get<SubtypeG>();
				case 7:
					return get<SubtypeH>() == rhs.get<SubtypeH>();
				case 8:
					return get<SubtypeI>() == rhs.get<SubtypeI>();
				case 9:
					return get<SubtypeJ>() == rhs.get<SubtypeJ>();
				case 10:
					return get<SubtypeK>() == rhs.get<SubtypeK>();
				case 11:
					return get<SubtypeL>() == rhs.get<SubtypeL>();
				case 12:
					return get<SubtypeM>() == rhs.get<SubtypeM>();
				case 13:
					return get<SubtypeN>() == rhs.get<SubtypeN>();
				case 14:
					return get<SubtypeO>() == rhs.get<SubtypeO>();
				case 15:
					return get<SubtypeP>() == rhs.get<SubtypeP>();
				default:
					break;
				};
			}

			return false;
		}

	private:

		union
		{
			SubtypeA a;
			SubtypeB b;
			SubtypeC c;
			SubtypeD d;
			SubtypeE e;
			SubtypeF f;
			SubtypeG g;
			SubtypeH h;
			SubtypeI i;
			SubtypeJ j;
			SubtypeK k;
			SubtypeL l;
			SubtypeM m;
			SubtypeN n;
			SubtypeO o;
			SubtypeP p;
		} _values;

		int8 _currentIndex;

	private:

		template<typename Subtype, typename PointerType>
		static void get(
			const Union& u,
			int32& index,
			PointerType& type
		)
		{
			if (AreTypesEqual<SubtypeA, Subtype>::Value)
			{
				index = 0;
				type = (PointerType) &u._values.a;
			}
			else if (AreTypesEqual<SubtypeB, Subtype>::Value)
			{
				index = 1;
				type = (PointerType) &u._values.b;
			}
			else if (AreTypesEqual<SubtypeC, Subtype>::Value)
			{
				index = 2;
				type = (PointerType) &u._values.c;
			}
			else if (AreTypesEqual<SubtypeD, Subtype>::Value)
			{
				index = 3;
				type = (PointerType) &u._values.d;
			}
			else if (AreTypesEqual<SubtypeE, Subtype>::Value)
			{
				index = 4;
				type = (PointerType) &u._values.e;
			}
			else if (AreTypesEqual<SubtypeF, Subtype>::Value)
			{
				index = 5;
				type = (PointerType) &u._values.f;
			}
			else if (AreTypesEqual<SubtypeG, Subtype>::Value)
			{
				index = 6;
				type = (PointerType) &u._values.g;
			}
			else if (AreTypesEqual<SubtypeH, Subtype>::Value)
			{
				index = 7;
				type = (PointerType) &u._values.h;
			}
			else if (AreTypesEqual<SubtypeI, Subtype>::Value)
			{
				index = 8;
				type = (PointerType) &u._values.i;
			}
			else if (AreTypesEqual<SubtypeJ, Subtype>::Value)
			{
				index = 9;
				type = (PointerType) &u._values.j;
			}
			else if (AreTypesEqual<SubtypeK, Subtype>::Value)
			{
				index = 10;
				type = (PointerType) &u._values.k;
			}
			else if (AreTypesEqual<SubtypeL, Subtype>::Value)
			{
				index = 11;
				type = (PointerType) &u._values.l;
			}
			else if (AreTypesEqual<SubtypeM, Subtype>::Value)
			{
				index = 12;
				type = (PointerType) &u._values.m;
			}
			else if (AreTypesEqual<SubtypeN, Subtype>::Value)
			{
				index = 13;
				type = (PointerType) &u._values.n;
			}
			else if (AreTypesEqual<SubtypeO, Subtype>::Value)
			{
				index = 14;
				type = (PointerType) &u._values.o;
			}
			else if (AreTypesEqual<SubtypeP, Subtype>::Value)
			{
				index = 15;
				type = (PointerType) &u._values.p;
			}
			else
			{
				
				index = -1;
				type = nullptr;
			}
		}
	};
}

#endif // !UNION_H_