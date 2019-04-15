#ifndef COLOR_H_
#define COLOR_H_

#include "GraphicsDefines.h"

#include "Types.h"
#include "Vector4.h"

namespace Arcana
{
	enum class GammaSpace
	{
		Linear,
		Pow22,
		sRGB,
	};

	class ARCANA_GRAPHICS_API Color;

	class ARCANA_GRAPHICS_API LinearColor
	{
	public:

		float R, G, B, A;

	public:

		LinearColor();

		LinearColor(float r, float g, float b, float a = 1.0f);

		LinearColor(const Color& color);

		LinearColor(const Vector4f& vector);

		Color toRGBE() const;

		float& component(int32 index);

		const float& component(int32 index) const;

		LinearColor clamp(float min = 0.0f, float max = 1.0f) const;

		bool equals(const LinearColor& color, float tolerance = Math::EPSILON) const;

		LinearColor linearRGBToHSV() const;

		LinearColor HSVToLinearRGB() const;

		Color quantize() const;

		Color toColor(const bool sRGB) const;

		LinearColor desaturate(float desaturation) const;

		float getMaxComponent() const;

		float getMinComponent() const;

		bool isNearBlack() const;

		float getLuminance() const;

		std::string toString() const;

	public:

		static const LinearColor Black;       ///< Black predefined color
		static const LinearColor Gray;		  ///< Gray predefined color
		static const LinearColor White;       ///< White predefined color
		static const LinearColor Red;         ///< Red predefined color
		static const LinearColor Green;       ///< Green predefined color
		static const LinearColor Blue;        ///< Blue predefined color
		static const LinearColor Yellow;      ///< Yellow predefined color
		static const LinearColor Magenta;     ///< Magenta predefined color
		static const LinearColor Cyan;        ///< Cyan predefined color
		static const LinearColor Transparent; ///< Transparent black predefined color

	public:

		static LinearColor fromSRGBColor(const Color& color);

		static LinearColor fromPow22Color(const Color& color);

		static LinearColor getHSV(uint8 hue, uint8 saturation, uint8 value);

		static LinearColor random();

		static LinearColor fromTemperature(double temperature);

		static float distance(const LinearColor& c1, const LinearColor& c2);

		static LinearColor lerp(const LinearColor& a, const LinearColor& b, double t);

		//bezier????

		//lerp hsv???

	public:

		LinearColor operator+(const LinearColor& color) const;

		LinearColor& operator+=(const LinearColor& color);

		LinearColor operator-(const LinearColor& color) const;

		LinearColor& operator-=(const LinearColor& color);

		LinearColor operator*(const LinearColor& color) const;

		LinearColor& operator*=(const LinearColor& color);

		LinearColor operator*(float scalar) const;

		LinearColor& operator*=(float scalar);

		LinearColor operator/(const LinearColor& color) const;

		LinearColor& operator/=(const LinearColor& color);

		LinearColor operator/(float scalar) const;

		LinearColor& operator/=(float scalar);

		bool operator==(const LinearColor& rhs) const;

		bool operator!=(const LinearColor& rhs) const;	

	private:

		static float __pow22Table[256];

		static float __sRGBToLinearTable[256];

		static const float __oneOver255;
	};

	class ARCANA_GRAPHICS_API Color
	{
	public:

		uint8 R, G, B, A;

	public:

		Color();

		Color(uint8 r, uint8 g, uint8 b, uint8 a = 255);

		Color(uint32 color);

		~Color();

		LinearColor fromRGBE() const;

		Color withAlpha(uint8 alpha) const;

		LinearColor asLinear() const;

		std::string toHexString() const;

		std::string toString() const;

	public:

		static Color fromHex(const std::string& hexString);

		static Color random();

		static Color fromTemperature(double temperature);

		static Color scalarRedGreen(float scalar);

		static Color fromString(std::string string);

	public:

		bool operator==(const Color &rhs) const;

		bool operator!=(const Color& rhs) const;

		void operator+=(const Color& rhs);

	};

	class ARCANA_GRAPHICS_API ColorTemperatureTable
	{
	public:

		static const int32 NumColors;
		static const double MaxTemperature; //kelvin
		static const double TemperatureScale;

		static Color getColor(double temp);

	private:

		static Color __blackBodyColors[1024];
	};
}

#endif // !COLOR_H_
