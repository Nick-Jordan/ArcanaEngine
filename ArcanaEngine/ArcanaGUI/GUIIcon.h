#ifndef GUI_ICON_H_
#define GUI_ICON_H_

#include "GUIDefines.h"

#include "Object.h"
#include "Texture.h"
#include <string>

namespace Arcana
{
	class ARCANA_GUI_API GUIIcon : public Object
	{
	public:

		GUIIcon(const std::string& path);

		~GUIIcon();

		const std::string& getPath() const;

		Texture* getTexture() const;

		uint32 getWidth() const;

		uint32 getHeight() const;

	private:

		void initialize();

	private:

		Texture* _texture;
		std::string _path;
		Vector2i _size;
	};
}

#endif // !GUI_ICON_H_