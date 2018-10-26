#ifndef SHAPE_H_
#define SHAPE_H_

#include "GraphicsDefines.h"

#include "Mesh.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API ObjectRenderer;

	class ARCANA_GRAPHICS_API Shape : public Object
	{
	public:

		virtual ~Shape();

		Mesh* getMesh();

		Shape& operator=(const Shape& shape);

	protected:

		Shape();

		void setMesh(Mesh* mesh);

	private:

		Mesh* _mesh;
	};

}

#endif // !SHAPE_H_
