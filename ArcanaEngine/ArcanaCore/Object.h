#ifndef OBJECT_H_
#define OBJECT_H_

#include "CoreDefines.h"

#include <string>

namespace Arcana
{
	/** \brief The base class for all core classes.
	 *
	 */

	class ARCANA_CORE_API Object
	{
	public:

		/** \brief Object default constructor.
		 */

		Object();

		/** \brief Object constructor with type argument.
		 */

		Object(const std::string& type);

		/** \brief Object destructor (should be virtual?)
		 */

		~Object();

		/** \brief Accessor for the object type.
		 */

		const std::string& getType() const;

	private:

		std::string _type;  ///< The object's type string.
	};

}

#endif // !OBJECT_H_

