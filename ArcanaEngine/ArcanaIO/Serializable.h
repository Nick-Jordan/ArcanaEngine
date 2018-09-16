#ifndef SERIALIZABLE_H_
#define SERIALIZABLE_H_

#include "IODefines.h"

#include "Archive.h"

namespace Arcana
{
	
	class Serializable
	{
	public:
	
		virtual ~Serializable() {}
				
		virtual void writeObject(Archive& archive) = 0;
		
		virtual void readObject(Archive& archive) = 0;
		
		virtual int64 getObjectSize() = 0;
	};
	
}

#endif // !SERIALIZABLE_H_