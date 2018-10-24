#ifndef SHAPE_H_
#define SHAPE_H_

#include "GraphicsDefines.h"

#include "Mesh.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API ObjectRenderer;

	class ARCANA_GRAPHICS_API Shape
	{
	public:

		virtual ~Shape();

		Mesh* getMesh();

		void* getUserData() const;

		void setUserData(void* userData);


		Shape& operator=(const Shape& shape);

	protected:

		Shape(Mesh* mesh);

	private:

		Mesh* _mesh;

		void* _userData;
	};

}

#endif // !SHAPE_H_
