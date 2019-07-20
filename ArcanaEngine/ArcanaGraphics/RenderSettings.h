#ifndef RENDER_SETTINGS_H
#define RENDER_SETTINGS_H_

#include "Types.h"

namespace Arcana
{
	struct RenderSettings
	{
		enum Attribute : uint32
		{
			Default = 0,
			Core = 1 << 0,
			Debug = 1 << 2
		};

		uint32 depthBits;
		uint32 stencilBits;
		uint32 alphaBits;

		uint32 majorVersion;
		uint32 minorVersion;

		uint32 sRgb;

		uint32 bitsPerPixel;
		uint32 antialiasingLevel;

		uint32 attributeFlags;

		double splashScreenLength; //keep?

		RenderSettings(uint32 bitsPerPixel = 0, uint32 depth = 0, uint32 stencil = 0, uint32 antialiasing = 0, uint32 major = 1, uint32 minor = 1, uint32 attributes = Default, bool sRgb = false, double splashScreenLength = 0.0) :
			bitsPerPixel(bitsPerPixel),
			depthBits(depth),
			stencilBits(stencil),
			antialiasingLevel(antialiasing),
			majorVersion(major),
			minorVersion(minor),
			attributeFlags(attributes),
			sRgb(sRgb),
			splashScreenLength(splashScreenLength)
		{
		}
	};
}

#endif // !RENDER_SETTINGS_H_