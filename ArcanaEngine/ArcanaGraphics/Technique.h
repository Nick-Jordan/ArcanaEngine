#ifndef TECHNIQUE_H_
#define TECHNIQUE_H_

#include "GraphicsDefines.h"

#include "Types.h"
#include "Shader.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API Material;

	class ARCANA_GRAPHICS_API Technique : public Object
	{
	public:
		
		Technique();

		Technique(uint32 numPasses);

		Technique(const Shader& shader);

		Technique(const Technique& technique);

		~Technique();

		uint32 getPassCount() const;

		Shader* getPass(uint32 index);

		void setPass(uint32 index, const Shader& pass);


		bool operator == (const Technique& t) const;

		bool operator != (const Technique& t) const;

		Technique& operator=(const Technique& technique);

	private:

		Shader* _passes;

		uint32 _numPasses;
	};

}

#endif // !TECHNIQUE_H_

