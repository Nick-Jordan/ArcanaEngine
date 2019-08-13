#ifndef KEY_VALUE_PAIR_H_
#define KEY_VALUE_PAIR_H_

namespace Arcana
{
	/** \brief Key/value pairs that are easy to use with custom containers.
	 *  This class acts the same as the standard pair.
	 */
	template <typename KeyType, typename ValueType>
	struct KeyValuePair
	{
		/** \brief Default constructor.
		 *  Creates a pair with an uninitialized key and an uninitialized value.
		 */
		KeyValuePair()
		{
		}

		/** \brief Constructor taking a key and a value.
		 */
		KeyValuePair(const KeyType& key, const ValueType& value)
			: key(key), value(value)
		{
		}

		/** \brief Constructor taking just a key.
		 *  Creates a pair with an uninitialized value.
		 */
		KeyValuePair(const KeyType& key)
			: key(key)
		{
		}

		/** \brief Equals operator.
		 *  Only checks key equality.
		 */
		bool operator==(const KeyValuePair& other) const
		{
			return key == other.key;
		}

		/** \brief Inequality operator.
		 *  Only checks key inequality.
		 */
		bool operator!=(const KeyValuePair& other) const
		{
			return key != other.key;
		}

		/** \brief Less than operator.
		 *  Only checks if this key is less than the other key.
		 */
		bool operator<(const KeyValuePair& other) const
		{
			return key < other.key;
		}

		/** \brief Functor operator taking two different KeyValuePairs.
		 *  Checks if this key is less than the other key.
		 */
		bool operator()(const KeyValuePair& A, const KeyValuePair& B) const
		{
			return A.key < B.key;
		}

		KeyType key;
		ValueType value;
	};
}

#endif // !KEY_VALUE_PAIR_H_