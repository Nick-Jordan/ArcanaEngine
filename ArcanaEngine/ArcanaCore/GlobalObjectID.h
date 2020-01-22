#ifndef GLOBAL_OBJECT_ID_H_
#define GLOBAL_OBJECT_ID_H_

#include "CoreDefines.h"

#include <string>
#include "Types.h"

#include "UUID.h"

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

		static GlobalObjectID Empty;

		/** \brief GlobalObjectID default constructor.
		 */
		GlobalObjectID();

		/** \brief GlobalObjectID constructor with name argument.
		 */
		GlobalObjectID(const std::string& name);

		/** \brief GlobalObjectID destructor.
		 */
		~GlobalObjectID();

		/** \brief Accessor for the object's name. 
	     */
		const std::string& getName() const;

		const std::string& getIdString() const;

		//change to custom uuid (probably wrapper for this library or something)
		const UUID& getId() const;

		bool isEmpty() const;

		/** \brief Relational equivalence operator.
		 *  Compares the GlobalObjectIDs' integer ids, rather than the names.
		 */
		bool operator==(const GlobalObjectID& other) const;

	private:

		std::string _name;  ///< The object id's name.
		UUID _id;          ///< The object id's integer id.

		static std::unordered_map<std::string, UUID> __uuidMap;
	};

}

#endif

