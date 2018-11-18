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

		bool needsMaterialAttributes() const;

		void needsMaterialAttributes(bool needsMaterialAttributes);


		bool operator == (const Technique& t) const;

		bool operator != (const Technique& t) const;

		Technique& operator=(const Technique& technique);

	protected:

		Shader* _passes;

		uint32 _numPasses;

		bool _needsMaterialAttributes;
	};

}

#endif // !TECHNIQUE_H_

