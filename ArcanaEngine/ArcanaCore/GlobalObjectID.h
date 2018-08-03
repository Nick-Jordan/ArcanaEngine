#ifndef GLOBAL_OBJECT_ID_H_
#define GLOBAL_OBJECT_ID_H_

#include "CoreDefines.h"

#include <string>
#include "Types.h"

namespace Arcana
{

	/** \brief A global object id.
	 *
	 *  This class takes a name string and hashes it.
	 *  This allows object ids to be compared using the hashed value, eliminating the need for string comparisons.
	 */

	class ARCANA_CORE_API GlobalObjectID
	{
	public:

		/** \brief GlobalObjectID default constructor.
		 */

		GlobalObjectID();

		/** \brief GlobalObjectID constructor with name argument.
		 */

		GlobalObjectID(const std::string& name);

		/** \brief GlobalObjectID constructor with both name and id arguments.
		 *  This constructor can be dangerous if the user creates two GlobalObjectIDs with the same integer id.
		 *  Two GlobalObjectIDs with different names would be equal.
		 */

		GlobalObjectID(const std::string& name, int64 id);

		/** \brief GlobalObjectID destructor.
		 */

		~GlobalObjectID();

		/** \brief Hashes a string. Converts a string into a 64-bit integer.
		 */

		int64 hashString(const std::string& string);

		/** \brief Accessor for the object's name. 
	     */

		const std::string& getName() const;

		/** \brief Accessor for the object's integer id.
		 */

		int64 getId() const;

		/** \brief Relational equivalence operator.
		 *  Compares the GlobalObjectIDs' integer ids, rather than the names.
		 */

		bool operator==(const GlobalObjectID& other);

	private:

		std::string _name;  ///< The object id's name.
		int64 _id;          ///< The object id's integer id.
	};

}

#endif

