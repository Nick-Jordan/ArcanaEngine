#ifndef XML_RESOURCE_DATABASE_H_
#define XML_RESOURCE_DATABASE_H_

#include "ResourceDefines.h"

#include "ResourceDatabase.h"

namespace Arcana
{
	class ARCANA_RESOURCE_API FindXMLResourceTask : public FindResourceTask
	{
	public:

		FindXMLResourceTask(Array<Resource>& array, const GlobalObjectID& id);

		virtual void run() override;

		virtual void done() override;

	private:

		Array<Resource>& _array;
		const GlobalObjectID _id;
	};

	class ARCANA_RESOURCE_API XMLResourceDatabase : public ResourceDatabase
	{
	public:

		static XMLResourceDatabase* create(const std::string& filename);

		~XMLResourceDatabase();

		virtual bool initialize() override;

		virtual bool finalize() override;

		virtual FindResourceTask* getResource(const GlobalObjectID& id) override;

		virtual Resource* findResource(const GlobalObjectID& id) override;

	private:

		XMLResourceDatabase();

	private:

		std::string _filename;

		Array<Resource> _resources;
	};

}

#endif // !XML_RESOURCE_DATABASE_H_

