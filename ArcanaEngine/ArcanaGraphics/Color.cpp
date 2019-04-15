#include "Color.h"

#include "Random.h"

namespace Arcana
{

	//Linear Color

	const LinearColor LinearColor::Black = LinearColor(0.0f, 0.0f, 0.0f, 1.0f);
	const LinearColor LinearColor::Gray = LinearColor(0.5f, 0.5f, 0.5f, 1.0f);
	const LinearColor LinearColor::White = LinearColor(1.0f, 1.0f, 1.0f, 1.0f);
	const LinearColor LinearColor::Red = LinearColor(1.0f, 0.0f, 0.0f, 1.0f);
	const LinearColor LinearColor::Green = LinearColor(0.0f, 1.0f, 0.0f, 1.0f);
	const LinearColor LinearColor::Blue = LinearColor(0.0f, 0.0f, 1.0f, 1.0f);
	const LinearColor LinearColor::Yellow = LinearColor(1.0f, 1.0f, 0.0f, 1.0f);
	const LinearColor LinearColor::Magenta = LinearColor(1.0f, 0.0f, 1.0f, 1.0f);
	const LinearColor LinearColor::Cyan = LinearColor(0.0f, 1.0f, 1.0f, 1.0f);
	const LinearColor LinearColor::Transparent = LinearColor(0.0f, 0.0f, 0.0f, 0.0f);

	LinearColor::LinearColor() : R(0.0f), G(0.0f), B(0.0f), A(0.0f)
	{

	}

	LinearColor::LinearColor(float r, float g, float b, float a) : R(r), G(g), B(b), A(a)
	{

	}

	LinearColor::LinearColor(const Color& color)
	{
		R = __sRGBToLinearTable[color.R];
		G = __sRGBToLinearTable[color.G];
		B = __sRGBToLinearTable[color.B];
		A = float(color.A) * __oneOver255;
	}

	LinearColor::LinearColor(const Vector4f& vector) : R(vector.x), G(vector.y), B(vector.z), A(1.0f)
	{

	}

	Color LinearColor::toRGBE() const
	{
		const float	largest = Math::max(Math::max(R, G), B);

		if (largest < Math::EPSILON)
		{
			return Color(0, 0, 0, 0);
		}
		

		int32 exponent;
		const float scale = frexp(largest, &exponent) / largest * 255.0f;

		return Color(
			Math::clamp((int32)(R * scale), 0, 255),
			Math::clamp((int32)(G * scale), 0, 255),
			Math::clamp((int32)(B * scale), 0, 255),
			Math::clamp(exponent, -128, 127) + 128
		);
	}

	float& LinearColor::component(int32 index)
	{
		return (&R)[index];
	}

	const float& LinearColor::component(int32 index) const
	{
		return (&R)[index];
	}

	LinearColor LinearColor::clamp(float min, float max) const
	{
		return LinearColor(
			Math::clamp(R, min, max),
			Math::clamp(G, min, max),
			Math::clamp(B, min, max),
			Math::clamp(A, min, max)
		);
	}

	bool LinearColor::equals(const LinearColor& color, float tolerance) const
	{
		return Math::abs(R - color.R) < tolerance 
			&& Math::abs(G - color.G) < tolerance 
			&& Math::abs(B - color.B) < tolerance 
			&& Math::abs(A - color.A) < tolerance;
	}

	LinearColor LinearColor::linearRGBToHSV() const
	{
		const float min = getMinComponent();
		const float max = getMaxComponent();
		const float range = max - min;

		const float hue = (max == min ? 0.0f :
			max == R ? fmod((((G - B) / range) * 60.0f) + 360.0f, 360.0f) :
			max == G ? (((B - R) / range) * 60.0f) + 120.0f :
			max == B ? (((R - G) / range) * 60.0f) + 240.0f :
			0.0f);

		const float saturation = (max == 0.0f ? 0.0f : range / max);
		const float value = max;

		return LinearColor(hue, saturation, value, A);
	}

	LinearColor LinearColor::HSVToLinearRGB() const
	{
		const float hue = R;
		const float saturation = G;
		const float value = B;

		const float h1 = hue / 60.0f;
		const float h1f = floorf(h1);
		const float h1fr = h1 - h1f;

		const float values[4] = 
		{
			value,
			value * (1.0f - saturation),
			value * (1.0f - (h1fr * saturation)),
			value * (1.0f - ((1.0f - h1fr) * saturation)),
		};
		const uint32 swizzle[6][3] = 
		{
			{0, 3, 1},
			{2, 0, 1},
			{1, 0, 3},
			{1, 2, 0},
			{3, 1, 0},
			{0, 1, 2},
		};
		const uint32 index = ((uint32)h1fr) % 6;

		return LinearColor(
			values[swizzle[index][0]],
			values[swizzle[index][1]],
			values[swizzle[index][2]],
			A
		);
	}

	Color LinearColor::quantize() const
	{
		return Color(
			(uint8)Math::clamp((int32)(R*255.0f), 0, 255),
			(uint8)Math::clamp((int32)(G*255.0f), 0, 255),
			(uint8)Math::clamp((int32)(B*255.0f), 0, 255),
			(uint8)Math::clamp((int32)(A*255.0f), 0, 255)
		);
	}

	Color LinearColor::toColor(const bool sRGB) const
	{
		float r = Math::clamp(R, 0.0f, 1.0f);
		float g = Math::clamp(G, 0.0f, 1.0f);
		float b = Math::clamp(B, 0.0f, 1.0f);
		float a = Math::clamp(A, 0.0f, 1.0f);

		if (sRGB)
		{
			r = r <= 0.0031308f ? r * 12.92f : powf(r, 1.0f / 2.4f) * 1.055f - 0.055f;
			g = g <= 0.0031308f ? g * 12.92f : powf(g, 1.0f / 2.4f) * 1.055f - 0.055f;
			b = b <= 0.0031308f ? b * 12.92f : powf(b, 1.0f / 2.4f) * 1.055f - 0.055f;
		}

		return Color(
			(int32)floorf(r * 255.999f), 
			(int32)floorf(g * 255.999f),
			(int32)floorf(b * 255.999f),
			(int32)floorf(a * 255.999f)
			);
	}

	LinearColor LinearColor::desaturate(float desaturation) const
	{
		float luminance = getLuminance();

		return lerp(*this, LinearColor(luminance, luminance, luminance, 0.0f), desaturation);
	}

	float LinearColor::getMaxComponent() const
	{
		return Math::max(Math::max(Math::max(R, G), B), A);
	}

	float LinearColor::getMinComponent() const
	{
		return Math::min(Math::min(Math::min(R, G), B), A);
	}

	bool LinearColor::isNearBlack() const
	{
		return Math::square(R) < Math::EPSILON 
			&& Math::square(G) < Math::EPSILON
			&& Math::square(B) < Math::EPSILON;
	}

	float LinearColor::getLuminance() const
	{
		return R * 0.3f + G * 0.59f + B * 0.11f;
	}

	std::string LinearColor::toString() const
	{
		return "linear color";
	}

	LinearColor LinearColor::fromSRGBColor(const Color& color)
	{
		return LinearColor(color);
	}

	LinearColor LinearColor::fromPow22Color(const Color& color)
	{
		return LinearColor(
			__pow22Table[color.R],
			__pow22Table[color.G],
			__pow22Table[color.B],
			float(color.A) * __oneOver255
		);
	}

	LinearColor LinearColor::getHSV(uint8 hue, uint8 saturation, uint8 value)
	{
		float brightness = value * 1.4f / 255.f;
		brightness *= 0.7f / (0.01f + sqrt(brightness));
		brightness = Math::clamp(brightness, 0.0f, 1.0f);
		const Vector3f hueVector = 
			(hue < 86) ? Vector3f((85 - hue) / 85.f, (hue - 0) / 85.f, 0) : 
			(hue < 171) ? Vector3f(0, (170 - hue) / 85.f, (hue - 85) / 85.f) : 
			Vector3f((hue - 170) / 85.f, 0, (255 - hue) / 84.f);
		const Vector3f color = (hueVector + saturation / 255.f * (Vector3f::one() - hue)) * brightness;
		return LinearColor(color.x, color.y, color.z, 1.0f);
	}

	LinearColor LinearColor::random()
	{
		const uint8 hue = (uint8) Random<float>::random(0.0f, 255.0f);
		return LinearColor::getHSV(hue, 0, 255);
	}

	LinearColor LinearColor::fromTemperature(double temperature)
	{
		temperature = Math::clamp(temperature, 0.0, 30000.0);//Math::clamp(temperature, 1000.0f, 15000.0f);

		double u = (0.860117757 + 1.54118254e-4 * temperature + 1.28641212e-7 * temperature*temperature) / (1.0 + 8.42420235e-4 * temperature + 7.08145163e-7 * temperature*temperature);
		double v = (0.317398726 + 4.22806245e-5 * temperature + 4.20481691e-8 * temperature*temperature) / (1.0 - 2.89741816e-5 * temperature + 1.61456053e-7 * temperature*temperature);

		double x = 3.0 * u / (2.0 * u - 8.0 * v + 4.0);
		double y = 2.0 * v / (2.0 * u - 8.0 * v + 4.0);
		double z = 1.0 - x - y;

		double Y = 1.0;
		double X = Y / y * x;
		double Z = Y / y * z;

		float R = 3.2404542f * X + -1.5371385f * Y + -0.4985314f * Z;
		float G = -0.9692660f * X + 1.8760108f * Y + 0.0415560f * Z;
		float B = 0.0556434f * X + -0.2040259f * Y + 1.0572252f * Z;

		return LinearColor(R, G, B);
	}

	float LinearColor::distance(const LinearColor& c1, const LinearColor& c2)
	{
		return sqrt(Math::square(c2.R - c1.R) + Math::square(c2.G - c1.G) + Math::square(c2.B - c1.B) + Math::square(c2.A - c1.A));
	}

	LinearColor LinearColor::lerp(const LinearColor& a, const LinearColor& b, double t)
	{
		return LinearColor(
			Math::lerp(a.R, b.R, t),
			Math::lerp(a.G, b.G, t),
			Math::lerp(a.B, b.B, t),
			Math::lerp(a.A, b.A, t)
		);
	}

	LinearColor LinearColor::operator+(const LinearColor& color) const
	{
		return LinearColor(
			R + color.R,
			G + color.G,
			B + color.B,
			A + color.A
		);
	}

	LinearColor& LinearColor::operator+=(const LinearColor& color)
	{
		R += color.R;
		G += color.G;
		B += color.B;
		A += color.A;
		return *this;
	}

	LinearColor LinearColor::operator-(const LinearColor& color) const
	{
		return LinearColor(
			R - color.R,
			G - color.G,
			B - color.B,
			A - color.A
		);
	}

	LinearColor& LinearColor::operator-=(const LinearColor& color)
	{
		R -= color.R;
		G -= color.G;
		B -= color.B;
		A -= color.A;
		return *this;
	}

	LinearColor LinearColor::operator*(const LinearColor& color) const
	{
		return LinearColor(
			R * color.R,
			G * color.G,
			B * color.B,
			A * color.A
		);
	}

	LinearColor& LinearColor::operator*=(const LinearColor& color)
	{
		R *= color.R;
		G *= color.G;
		B *= color.B;
		A *= color.A;
		return *this;
	}

	LinearColor LinearColor::operator*(float scalar) const
	{
		return LinearColor(
			R * scalar,
			G * scalar,
			B * scalar,
			A * scalar
		);
	}

	LinearColor& LinearColor::operator*=(float scalar)
	{
		R *= scalar;
		G *= scalar;
		B *= scalar;
		A *= scalar;
		return *this;
	}

	LinearColor LinearColor::operator/(const LinearColor& color) const
	{
		return LinearColor(
			R / color.R,
			G / color.G,
			B / color.B,
			A / color.A
		);
	}

	LinearColor& LinearColor::operator/=(const LinearColor& color)
	{
		R /= color.R;
		G /= color.G;
		B /= color.B;
		A /= color.A;
		return *this;
	}

	LinearColor LinearColor::operator/(float scalar) const
	{
		const float	inv = 1.0f / scalar;

		return LinearColor(
			R * inv,
			G * inv,
			B * inv,
			A * inv
		);
	}

	LinearColor& LinearColor::operator/=(float scalar)
	{
		const float	inv = 1.0f / scalar;

		R *= inv;
		G *= inv;
		B *= inv;
		A *= inv;
		return *this;
	}

	bool LinearColor::operator==(const LinearColor& rhs) const
	{
		return R == rhs.R && G == rhs.G && B == rhs.B && A == rhs.A;
	}

	bool LinearColor::operator!=(const LinearColor& rhs) const
	{
		return R != rhs.R && G != rhs.G && B != rhs.B && A != rhs.A;
	}

	float LinearColor::__pow22Table[256] =
	{
		0, 5.07705190066176E-06, 2.33280046660989E-05, 5.69217657121931E-05, 0.000107187362341244, 0.000175123977503027, 0.000261543754548491, 0.000367136269815943, 0.000492503787191433,
		0.000638182842167022, 0.000804658499513058, 0.000992374304074325, 0.0012017395224384, 0.00143313458967186, 0.00168691531678928, 0.00196341621339647, 0.00226295316070643,
		0.00258582559623417, 0.00293231832393836, 0.00330270303200364, 0.00369723957890013, 0.00411617709328275, 0.00455975492252602, 0.00502820345685554, 0.00552174485023966,
		0.00604059365484981, 0.00658495738258168, 0.00715503700457303, 0.00775102739766061, 0.00837311774514858, 0.00902149189801213, 0.00969632870165823, 0.0103978022925553,
		0.0111260823683832, 0.0118813344348137, 0.0126637200315821, 0.0134733969401426, 0.0143105193748841, 0.0151752381596252, 0.0160677008908869, 0.01698805208925, 0.0179364333399502,
		0.0189129834237215, 0.0199178384387857, 0.0209511319147811, 0.0220129949193365, 0.0231035561579214, 0.0242229420675342, 0.0253712769047346, 0.0265486828284729, 0.027755279978126,
		0.0289911865471078, 0.0302565188523887, 0.0315513914002264, 0.0328759169483838, 0.034230206565082, 0.0356143696849188, 0.0370285141619602, 0.0384727463201946, 0.0399471710015256,
		0.0414518916114625, 0.0429870101626571, 0.0445526273164214, 0.0461488424223509, 0.0477757535561706, 0.049433457555908, 0.0511220500564934, 0.052841625522879, 0.0545922772817603,
		0.0563740975519798, 0.0581871774736854, 0.0600316071363132, 0.0619074756054558, 0.0638148709486772, 0.0657538802603301, 0.0677245896854243, 0.0697270844425988, 0.0717614488462391,
		0.0738277663277846, 0.0759261194562648, 0.0780565899581019, 0.080219258736215, 0.0824142058884592, 0.0846415107254295, 0.0869012517876603, 0.0891935068622478, 0.0915183529989195,
		0.0938758665255778, 0.0962661230633397, 0.0986891975410945, 0.1011451642096, 0.103634096655137, 0.106156067812744, 0.108711149979039, 0.11129941482466, 0.113920933406333,
		0.116575776178572, 0.119264013005047, 0.121985713169619, 0.124740945387051, 0.127529777813422, 0.130352278056244, 0.1332085131843, 0.136098549737202, 0.139022453734703,
		0.141980290685736, 0.144972125597231, 0.147998022982685, 0.151058046870511, 0.154152260812165, 0.157280727890073, 0.160443510725344, 0.16364067148529, 0.166872271890766,
		0.170138373223312, 0.173439036332135, 0.176774321640903, 0.18014428915439, 0.183548998464951, 0.186988508758844, 0.190462878822409, 0.193972167048093, 0.19751643144034,
		0.201095729621346, 0.204710118836677, 0.208359655960767, 0.212044397502288, 0.215764399609395, 0.219519718074868, 0.223310408341127, 0.227136525505149, 0.230998124323267,
		0.23489525921588, 0.238827984272048, 0.242796353254002, 0.24680041960155, 0.2508402364364, 0.254915856566385, 0.259027332489606, 0.263174716398492, 0.267358060183772,
		0.271577415438375, 0.275832833461245, 0.280124365261085, 0.284452061560024, 0.288815972797219, 0.293216149132375, 0.297652640449211, 0.302125496358853, 0.306634766203158,
		0.311180499057984, 0.315762743736397, 0.32038154879181, 0.325036962521076, 0.329729032967515, 0.334457807923889, 0.339223334935327, 0.344025661302187, 0.348864834082879,
		0.353740900096629, 0.358653905926199, 0.363603897920553, 0.368590922197487, 0.373615024646202, 0.37867625092984, 0.383774646487975, 0.388910256539059, 0.394083126082829,
		0.399293299902674, 0.404540822567962, 0.409825738436323, 0.415148091655907, 0.420507926167587, 0.425905285707146, 0.43134021380741, 0.436812753800359, 0.442322948819202,
		0.44787084180041, 0.453456475485731, 0.45907989242416, 0.46474113497389, 0.470440245304218, 0.47617726539744, 0.481952237050698, 0.487765201877811, 0.493616201311074,
		0.49950527660303, 0.505432468828216, 0.511397818884879, 0.517401367496673, 0.523443155214325, 0.529523222417277, 0.535641609315311, 0.541798355950137, 0.547993502196972,
		0.554227087766085, 0.560499152204328, 0.566809734896638, 0.573158875067523, 0.579546611782525, 0.585972983949661, 0.592438030320847, 0.598941789493296, 0.605484299910907,
		0.612065599865624, 0.61868572749878, 0.625344720802427, 0.632042617620641, 0.638779455650817, 0.645555272444934, 0.652370105410821, 0.659223991813387, 0.666116968775851,
		0.673049073280942, 0.680020342172095, 0.687030812154625, 0.694080519796882, 0.701169501531402, 0.708297793656032, 0.715465432335048, 0.722672453600255, 0.729918893352071,
		0.737204787360605, 0.744530171266715, 0.751895080583051, 0.759299550695091, 0.766743616862161, 0.774227314218442, 0.781750677773962, 0.789313742415586, 0.796916542907978,
		0.804559113894567, 0.81224148989849, 0.819963705323528, 0.827725794455034, 0.835527791460841, 0.843369730392169, 0.851251645184515, 0.859173569658532, 0.867135537520905,
		0.875137582365205, 0.883179737672745, 0.891262036813419, 0.899384513046529, 0.907547199521614, 0.915750129279253, 0.923993335251873, 0.932276850264543, 0.940600707035753,
		0.948964938178195, 0.957369576199527, 0.96581465350313, 0.974300202388861, 0.982826255053791, 0.99139284359294, 1
	};

	float LinearColor::__sRGBToLinearTable[256] =
	{
		0,
		0.000303526983548838, 0.000607053967097675, 0.000910580950646512, 0.00121410793419535, 0.00151763491774419,
		0.00182116190129302, 0.00212468888484186, 0.0024282158683907, 0.00273174285193954, 0.00303526983548838,
		0.00334653564113713, 0.00367650719436314, 0.00402471688178252, 0.00439144189356217, 0.00477695332960869,
		0.005181516543916, 0.00560539145834456, 0.00604883284946662, 0.00651209061157708, 0.00699540999852809,
		0.00749903184667767, 0.00802319278093555, 0.0085681254056307, 0.00913405848170623, 0.00972121709156193,
		0.0103298227927056, 0.0109600937612386, 0.0116122449260844, 0.012286488094766, 0.0129830320714536,
		0.0137020827679224, 0.0144438433080002, 0.0152085141260192, 0.0159962930597398, 0.0168073754381669,
		0.0176419541646397, 0.0185002197955389, 0.0193823606149269, 0.0202885627054049, 0.0212190100154473,
		0.0221738844234532, 0.02315336579873, 0.0241576320596103, 0.0251868592288862, 0.0262412214867272,
		0.0273208912212394, 0.0284260390768075, 0.0295568340003534, 0.0307134432856324, 0.0318960326156814,
		0.0331047661035236, 0.0343398063312275, 0.0356013143874111, 0.0368894499032755, 0.0382043710872463,
		0.0395462347582974, 0.0409151963780232, 0.0423114100815264, 0.0437350287071788, 0.0451862038253117,
		0.0466650857658898, 0.0481718236452158, 0.049706565391714, 0.0512694577708345, 0.0528606464091205,
		0.0544802758174765, 0.0561284894136735, 0.0578054295441256, 0.0595112375049707, 0.0612460535624849,
		0.0630100169728596, 0.0648032660013696, 0.0666259379409563, 0.0684781691302512, 0.070360094971063,
		0.0722718499453493, 0.0742135676316953, 0.0761853807213167, 0.0781874210336082, 0.0802198195312533,
		0.0822827063349132, 0.0843762107375113, 0.0865004612181274, 0.0886555854555171, 0.0908417103412699,
		0.0930589619926197, 0.0953074657649191, 0.0975873462637915, 0.0998987273569704, 0.102241732185838,
		0.104616483176675, 0.107023102051626, 0.109461709839399, 0.1119324268857, 0.114435372863418,
		0.116970666782559, 0.119538426999953, 0.122138771228724, 0.124771816547542, 0.127437679409664,
		0.130136475651761, 0.132868320502552, 0.135633328591233, 0.138431613955729, 0.141263290050755,
		0.144128469755705, 0.147027265382362, 0.149959788682454, 0.152926150855031, 0.155926462553701,
		0.158960833893705, 0.162029374458845, 0.16513219330827, 0.168269398983119, 0.171441099513036,
		0.174647402422543, 0.17788841473729, 0.181164242990184, 0.184474993227387, 0.187820771014205,
		0.191201681440861, 0.194617829128147, 0.198069318232982, 0.201556252453853, 0.205078735036156,
		0.208636868777438, 0.212230756032542, 0.215860498718652, 0.219526198320249, 0.223227955893977,
		0.226965872073417, 0.23074004707378, 0.23455058069651, 0.238397572333811, 0.242281120973093,
		0.246201325201334, 0.250158283209375, 0.254152092796134, 0.258182851372752, 0.262250655966664,
		0.266355603225604, 0.270497789421545, 0.274677310454565, 0.278894261856656, 0.283148738795466,
		0.287440836077983, 0.291770648154158, 0.296138269120463, 0.300543792723403, 0.304987312362961,
		0.309468921095997, 0.313988711639584, 0.3185467763743, 0.323143207347467, 0.32777809627633,
		0.332451534551205, 0.337163613238559, 0.341914423084057, 0.346704054515559, 0.351532597646068,
		0.356400142276637, 0.361306777899234, 0.36625259369956, 0.371237678559833, 0.376262121061519,
		0.381326009488037, 0.386429431827418, 0.39157247577492, 0.396755228735618, 0.401977777826949,
		0.407240209881218, 0.41254261144808, 0.417885068796976, 0.423267667919539, 0.428690494531971,
		0.434153634077377, 0.439657171728079, 0.445201192387887, 0.450785780694349, 0.456411021020965,
		0.462076997479369, 0.467783793921492, 0.473531493941681, 0.479320180878805, 0.485149937818323,
		0.491020847594331, 0.496932992791578, 0.502886455747457, 0.50888131855397, 0.514917663059676,
		0.520995570871595, 0.527115123357109, 0.533276401645826, 0.539479486631421, 0.545724458973463,
		0.552011399099209, 0.558340387205378, 0.56471150325991, 0.571124827003694, 0.577580437952282,
		0.584078415397575, 0.590618838409497, 0.597201785837643, 0.603827336312907, 0.610495568249093,
		0.617206559844509, 0.623960389083534, 0.630757133738175, 0.637596871369601, 0.644479679329661,
		0.651405634762384, 0.658374814605461, 0.665387295591707, 0.672443154250516, 0.679542466909286,
		0.686685309694841, 0.693871758534824, 0.701101889159085, 0.708375777101046, 0.71569349769906,
		0.723055126097739, 0.730460737249286, 0.737910405914797, 0.745404206665559, 0.752942213884326,
		0.760524501766589, 0.768151144321824, 0.775822215374732, 0.783537788566466, 0.791297937355839,
		0.799102735020525, 0.806952254658248, 0.81484656918795, 0.822785751350956, 0.830769873712124,
		0.838799008660978, 0.846873228412837, 0.854992605009927, 0.863157210322481, 0.871367116049835,
		0.879622393721502, 0.887923114698241, 0.896269350173118, 0.904661171172551, 0.913098648557343,
		0.921581853023715, 0.930110855104312, 0.938685725169219, 0.947306533426946, 0.955973349925421,
		0.964686244552961, 0.973445287039244, 0.982250546956257, 0.991102093719252, 1.0,
	};

	const float LinearColor::__oneOver255 = 1.0f / 255.0f;


	//Color

	Color::Color() : R(0), G(0), B(0), A(0)
	{

	}

	Color::Color(uint8 r, uint8 g, uint8 b, uint8 a) : R(r), G(g), B(b), A(a)
	{

	}

	Color::Color(uint32 color) : 
		R((color & 0xff000000) >> 24),
		G((color & 0x00ff0000) >> 16),
		B((color & 0x0000ff00) >> 8),
		A((color & 0x000000ff) >> 0)
	{

	}

	Color::~Color()
	{

	}

	LinearColor Color::fromRGBE() const
	{
		if (A == 0)
		{
			return LinearColor::Black;
		}

		const float scale = ldexp(1 / 255.0, A - 128);
		return LinearColor(R * scale, G * scale, B * scale, 1.0f);
	}

	Color Color::withAlpha(uint8 alpha) const
	{
		return Color(R, G, B, alpha);
	}

	LinearColor Color::asLinear() const
	{
		return LinearColor(
			(float)R / 255.0f, 
			(float)G / 255.0f, 
			(float)B / 255.0f,
			(float)A / 255.0f
		);
	}

	std::string Color::toHexString() const
	{
		return "hex color";
	}

	std::string Color::toString() const
	{
		return "color";
	}

	Color Color::fromHex(const std::string& hexString)
	{
		//////////
		return Color();
	}

	Color Color::random()
	{
		return LinearColor::random().toColor(true);
	}

	Color Color::fromTemperature(double temperature)
	{
		return ColorTemperatureTable::getColor(temperature);
	}

	Color Color::scalarRedGreen(float scalar)
	{
		float red = Math::clamp((1.0f - scalar) / 0.5f, 0.f, 1.f);
		float green = Math::clamp((scalar / 0.5f), 0.f, 1.f);
		return Color((int32)(255 * red), (int32)(255 * green), 0, 255);
	}

	Color Color::fromString(std::string string)
	{
		////////
		return Color();
	}

	bool Color::operator==(const Color &rhs) const
	{
		return R == rhs.R && G == rhs.G && B == rhs.B && A == rhs.A;
	}

	bool Color::operator!=(const Color& rhs) const
	{
		return R != rhs.R && G != rhs.G && B != rhs.B && A != rhs.A;
	}

	void Color::operator+=(const Color& rhs)
	{
		R = (uint8)Math::min((int32)R + (int32)rhs.R, 255);
		G = (uint8)Math::min((int32)G + (int32)rhs.G, 255);
		B = (uint8)Math::min((int32)B + (int32)rhs.B, 255);
		A = (uint8)Math::min((int32)A + (int32)rhs.A, 255);
	}


	//ColorTemperatureTable

	const int32 ColorTemperatureTable::NumColors = 1024;

	const double ColorTemperatureTable::MaxTemperature = 30000.0;

	const double ColorTemperatureTable::TemperatureScale = (float)(NumColors - 1) / MaxTemperature;

	Color ColorTemperatureTable::getColor(double temp)
	{
		uint32 colorTableIndex = (uint32)(temp * TemperatureScale);
		if (colorTableIndex >= NumColors)
		{
			return __blackBodyColors[NumColors - 1];
		}

		return __blackBodyColors[colorTableIndex];
	}

	Color ColorTemperatureTable::__blackBodyColors[1024] =
	{
		Color(0, 0, 0),
		Color(4, 0, 0),
		Color(8, 0, 0),
		Color(13, 0, 0),
		Color(17, 0, 0),
		Color(21, 0, 0),
		Color(26, 0, 0),
		Color(30, 0, 0),
		Color(34, 0, 0),
		Color(38, 0, 0),
		Color(42, 0, 0),
		Color(47, 0, 0),
		Color(51, 0, 0),
		Color(55, 0, 0),
		Color(60, 0, 0),
		Color(64, 0, 0),
		Color(68, 0, 0),
		Color(72, 0, 0),
		Color(76, 0, 0),
		Color(81, 0, 0),
		Color(85, 0, 0),
		Color(89, 0, 0),
		Color(93, 0, 0),
		Color(97, 0, 0),
		Color(101, 0, 0),
		Color(105, 0, 0),
		Color(109, 0, 0),
		Color(113, 0, 0),
		Color(117, 0, 0),
		Color(121, 0, 0),
		Color(125, 0, 0),
		Color(130, 0, 0),
		Color(134, 0, 0),
		Color(138, 0, 0),
		Color(142, 0, 0),
		Color(146, 0, 0),
		Color(150, 0, 0),
		Color(154, 0, 0),
		Color(158, 0, 0),
		Color(162, 0, 0),
		Color(166, 0, 0),
		Color(170, 0, 0),
		Color(174, 0, 0),
		Color(178, 0, 0),
		Color(183, 0, 0),
		Color(187, 0, 0),
		Color(191, 0, 0),
		Color(196, 0, 0),
		Color(200, 0, 0),
		Color(204, 0, 0),
		Color(208, 0, 0),
		Color(212, 0, 0),
		Color(217, 0, 0),
		Color(221, 0, 0),
		Color(225, 0, 0),
		Color(230, 0, 0),
		Color(234, 0, 0),
		Color(238, 0, 0),
		Color(242, 0, 0),
		Color(246, 0, 0),
		Color(251, 0, 0),
		Color(255, 0, 0),
		Color(255, 4, 0),
		Color(255, 8, 0),
		Color(255, 12, 0),
		Color(255, 16, 0),
		Color(255, 20, 0),
		Color(255, 24, 0),
		Color(255, 28, 0),
		Color(255, 32, 0),
		Color(255, 36, 0),
		Color(255, 40, 0),
		Color(255, 44, 0),
		Color(255, 48, 0),
		Color(255, 52, 0),
		Color(255, 56, 0),
		Color(255, 60, 0),
		Color(255, 64, 0),
		Color(255, 68, 0),
		Color(255, 72, 0),
		Color(255, 76, 0),
		Color(255, 80, 0),
		Color(255, 84, 0),
		Color(255, 88, 0),
		Color(255, 92, 0),
		Color(255, 96, 0),
		Color(255, 100, 0),
		Color(255, 104, 0),
		Color(255, 108, 0),
		Color(255, 112, 0),
		Color(255, 116, 0),
		Color(255, 120, 0),
		Color(255, 124, 0),
		Color(255, 128, 0),
		Color(255, 132, 0),
		Color(255, 136, 0),
		Color(255, 140, 0),
		Color(255, 144, 0),
		Color(255, 148, 0),
		Color(255, 152, 0),
		Color(255, 156, 0),
		Color(255, 160, 0),
		Color(255, 164, 0),
		Color(255, 167, 7),
		Color(255, 169, 14),
		Color(255, 172, 20),
		Color(255, 175, 27),
		Color(255, 177, 33),
		Color(255, 180, 40),
		Color(255, 182, 47),
		Color(255, 185, 53),
		Color(255, 187, 60),
		Color(255, 190, 67),
		Color(255, 193, 73),
		Color(255, 195, 80),
		Color(255, 197, 86),
		Color(255, 200, 93),
		Color(255, 203, 100),
		Color(255, 205, 106),
		Color(255, 208, 113),
		Color(255, 211, 120),
		Color(255, 213, 126),
		Color(255, 215, 133),
		Color(255, 218, 139),
		Color(255, 221, 146),
		Color(255, 223, 153),
		Color(255, 226, 159),
		Color(255, 228, 166),
		Color(255, 231, 173),
		Color(255, 233, 179),
		Color(255, 236, 186),
		Color(255, 239, 192),
		Color(255, 241, 199),
		Color(255, 244, 206),
		Color(255, 244, 207),
		Color(255, 244, 208),
		Color(255, 245, 209),
		Color(255, 245, 210),
		Color(255, 245, 211),
		Color(255, 245, 212),
		Color(255, 246, 213),
		Color(255, 246, 214),
		Color(255, 246, 215),
		Color(255, 246, 216),
		Color(255, 247, 217),
		Color(255, 247, 218),
		Color(255, 247, 219),
		Color(255, 248, 220),
		Color(255, 248, 222),
		Color(255, 248, 222),
		Color(255, 248, 223),
		Color(255, 249, 224),
		Color(255, 249, 225),
		Color(255, 249, 226),
		Color(255, 249, 228),
		Color(255, 250, 229),
		Color(255, 250, 230),
		Color(255, 250, 231),
		Color(255, 250, 232),
		Color(255, 251, 232),
		Color(255, 251, 234),
		Color(255, 251, 235),
		Color(255, 252, 236),
		Color(255, 252, 237),
		Color(255, 252, 238),
		Color(255, 252, 239),
		Color(255, 253, 240),
		Color(255, 253, 241),
		Color(255, 253, 242),
		Color(255, 253, 243),
		Color(255, 254, 244),
		Color(255, 254, 245),
		Color(255, 254, 246),
		Color(255, 254, 247),
		Color(255, 255, 248),
		Color(255, 255, 248),
		Color(255, 255, 248),
		Color(255, 255, 248),
		Color(255, 255, 248),
		Color(255, 255, 249),
		Color(255, 255, 249),
		Color(255, 255, 249),
		Color(255, 255, 249),
		Color(255, 255, 249),
		Color(255, 255, 249),
		Color(255, 255, 249),
		Color(255, 255, 249),
		Color(255, 255, 249),
		Color(255, 255, 250),
		Color(255, 255, 250),
		Color(255, 255, 250),
		Color(255, 255, 250),
		Color(255, 255, 250),
		Color(255, 255, 250),
		Color(255, 255, 250),
		Color(255, 255, 250),
		Color(255, 255, 250),
		Color(255, 255, 251),
		Color(255, 255, 251),
		Color(255, 255, 251),
		Color(255, 255, 251),
		Color(255, 255, 251),
		Color(255, 255, 251),
		Color(255, 255, 251),
		Color(255, 255, 251),
		Color(255, 255, 252),
		Color(255, 255, 252),
		Color(255, 255, 252),
		Color(255, 255, 252),
		Color(255, 255, 252),
		Color(255, 255, 252),
		Color(255, 255, 252),
		Color(255, 255, 252),
		Color(255, 255, 252),
		Color(255, 255, 253),
		Color(255, 255, 253),
		Color(255, 255, 253),
		Color(255, 255, 253),
		Color(255, 255, 253),
		Color(255, 255, 253),
		Color(255, 255, 253),
		Color(255, 255, 253),
		Color(255, 255, 253),
		Color(255, 255, 254),
		Color(255, 255, 254),
		Color(255, 255, 254),
		Color(255, 255, 254),
		Color(255, 255, 254),
		Color(255, 255, 254),
		Color(255, 255, 254),
		Color(255, 255, 254),
		Color(255, 255, 254),
		Color(255, 255, 255),
		Color(255, 255, 255),
		Color(255, 255, 255),
		Color(255, 255, 255),
		Color(255, 255, 255),
		Color(253, 254, 255),
		Color(250, 253, 255),
		Color(248, 251, 255),
		Color(245, 250, 255),
		Color(243, 249, 255),
		Color(240, 248, 255),
		Color(238, 246, 255),
		Color(235, 245, 255),
		Color(233, 244, 255),
		Color(230, 243, 255),
		Color(228, 241, 255),
		Color(225, 240, 255),
		Color(223, 239, 255),
		Color(220, 238, 255),
		Color(218, 236, 255),
		Color(215, 235, 255),
		Color(213, 234, 255),
		Color(210, 233, 255),
		Color(208, 232, 255),
		Color(205, 230, 255),
		Color(203, 229, 255),
		Color(200, 228, 255),
		Color(198, 227, 255),
		Color(195, 225, 255),
		Color(193, 224, 255),
		Color(190, 223, 255),
		Color(188, 222, 255),
		Color(185, 220, 255),
		Color(183, 219, 255),
		Color(180, 218, 255),
		Color(178, 217, 255),
		Color(175, 215, 255),
		Color(173, 214, 255),
		Color(170, 213, 255),
		Color(168, 212, 255),
		Color(165, 211, 255),
		Color(163, 209, 255),
		Color(160, 208, 255),
		Color(158, 207, 255),
		Color(155, 206, 255),
		Color(153, 204, 255),
		Color(150, 203, 255),
		Color(148, 202, 255),
		Color(145, 201, 255),
		Color(143, 199, 255),
		Color(140, 198, 255),
		Color(138, 197, 255),
		Color(135, 196, 255),
		Color(133, 194, 255),
		Color(130, 193, 255),
		Color(128, 192, 255),
		Color(127, 192, 255),
		Color(126, 191, 255),
		Color(126, 191, 255),
		Color(125, 190, 255),
		Color(124, 190, 255),
		Color(123, 190, 255),
		Color(122, 189, 255),
		Color(122, 189, 255),
		Color(121, 188, 255),
		Color(120, 188, 255),
		Color(119, 188, 255),
		Color(119, 187, 255),
		Color(118, 187, 255),
		Color(117, 186, 255),
		Color(116, 186, 255),
		Color(115, 186, 255),
		Color(115, 185, 255),
		Color(114, 185, 255),
		Color(113, 184, 255),
		Color(112, 184, 255),
		Color(111, 184, 255),
		Color(111, 183, 255),
		Color(110, 183, 255),
		Color(109, 183, 255),
		Color(108, 182, 255),
		Color(107, 182, 255),
		Color(107, 181, 255),
		Color(106, 181, 255),
		Color(105, 181, 255),
		Color(104, 180, 255),
		Color(104, 180, 255),
		Color(103, 179, 255),
		Color(102, 179, 255),
		Color(101, 179, 255),
		Color(100, 178, 255),
		Color(100, 178, 255),
		Color(99, 177, 255),
		Color(98, 177, 255),
		Color(97, 177, 255),
		Color(96, 176, 255),
		Color(96, 176, 255),
		Color(95, 175, 255),
		Color(94, 175, 255),
		Color(93, 175, 255),
		Color(92, 174, 255),
		Color(92, 174, 255),
		Color(91, 173, 255),
		Color(90, 173, 255),
		Color(89, 173, 255),
		Color(88, 172, 255),
		Color(88, 172, 255),
		Color(87, 171, 255),
		Color(86, 171, 255),
		Color(85, 171, 255),
		Color(85, 170, 255),
		Color(84, 170, 255),
		Color(83, 169, 255),
		Color(82, 169, 255),
		Color(81, 169, 255),
		Color(81, 168, 255),
		Color(80, 168, 255),
		Color(79, 168, 255),
		Color(78, 167, 255),
		Color(77, 167, 255),
		Color(77, 166, 255),
		Color(76, 166, 255),
		Color(75, 166, 255),
		Color(74, 165, 255),
		Color(73, 165, 255),
		Color(73, 164, 255),
		Color(72, 164, 255),
		Color(71, 164, 255),
		Color(70, 163, 255),
		Color(70, 163, 255),
		Color(69, 162, 255),
		Color(68, 162, 255),
		Color(67, 162, 255),
		Color(66, 161, 255),
		Color(66, 161, 255),
		Color(65, 160, 255),
		Color(64, 160, 255),
		Color(64, 160, 255),
		Color(63, 160, 255),
		Color(63, 159, 255),
		Color(62, 159, 255),
		Color(62, 159, 255),
		Color(61, 159, 255),
		Color(61, 158, 255),
		Color(60, 158, 255),
		Color(60, 158, 255),
		Color(60, 158, 255),
		Color(59, 158, 255),
		Color(59, 157, 255),
		Color(58, 157, 255),
		Color(58, 157, 255),
		Color(57, 157, 255),
		Color(57, 156, 255),
		Color(56, 156, 255),
		Color(56, 156, 255),
		Color(56, 156, 255),
		Color(55, 156, 255),
		Color(55, 155, 255),
		Color(54, 155, 255),
		Color(54, 155, 255),
		Color(53, 155, 255),
		Color(53, 154, 255),
		Color(52, 154, 255),
		Color(52, 154, 255),
		Color(52, 154, 255),
		Color(51, 154, 255),
		Color(51, 153, 255),
		Color(50, 153, 255),
		Color(50, 153, 255),
		Color(49, 153, 255),
		Color(49, 152, 255),
		Color(48, 152, 255),
		Color(48, 152, 255),
		Color(48, 152, 255),
		Color(47, 152, 255),
		Color(47, 151, 255),
		Color(46, 151, 255),
		Color(46, 151, 255),
		Color(45, 151, 255),
		Color(45, 150, 255),
		Color(44, 150, 255),
		Color(44, 150, 255),
		Color(44, 150, 255),
		Color(43, 150, 255),
		Color(43, 149, 255),
		Color(42, 149, 255),
		Color(42, 149, 255),
		Color(41, 149, 255),
		Color(41, 148, 255),
		Color(40, 148, 255),
		Color(40, 148, 255),
		Color(40, 148, 255),
		Color(39, 148, 255),
		Color(39, 147, 255),
		Color(38, 147, 255),
		Color(38, 147, 255),
		Color(37, 147, 255),
		Color(37, 146, 255),
		Color(36, 146, 255),
		Color(36, 146, 255),
		Color(36, 146, 255),
		Color(35, 146, 255),
		Color(35, 145, 255),
		Color(34, 145, 255),
		Color(34, 145, 255),
		Color(33, 145, 255),
		Color(33, 144, 255),
		Color(32, 144, 255),
		Color(32, 144, 255),
		Color(32, 144, 255),
		Color(31, 144, 255),
		Color(31, 143, 255),
		Color(30, 143, 255),
		Color(30, 143, 255),
		Color(29, 143, 255),
		Color(29, 142, 255),
		Color(28, 142, 255),
		Color(28, 142, 255),
		Color(28, 142, 255),
		Color(27, 142, 255),
		Color(27, 141, 255),
		Color(26, 141, 255),
		Color(26, 141, 255),
		Color(25, 141, 255),
		Color(25, 140, 255),
		Color(24, 140, 255),
		Color(24, 140, 255),
		Color(24, 140, 255),
		Color(23, 140, 255),
		Color(23, 139, 255),
		Color(22, 139, 255),
		Color(22, 139, 255),
		Color(21, 139, 255),
		Color(21, 138, 255),
		Color(20, 138, 255),
		Color(20, 138, 255),
		Color(20, 138, 255),
		Color(19, 138, 255),
		Color(19, 137, 255),
		Color(18, 137, 255),
		Color(18, 137, 255),
		Color(17, 137, 255),
		Color(17, 136, 255),
		Color(16, 136, 255),
		Color(16, 136, 255),
		Color(16, 136, 255),
		Color(15, 136, 255),
		Color(15, 135, 255),
		Color(14, 135, 255),
		Color(14, 135, 255),
		Color(13, 135, 255),
		Color(13, 134, 255),
		Color(12, 134, 255),
		Color(12, 134, 255),
		Color(12, 134, 255),
		Color(11, 134, 255),
		Color(11, 133, 255),
		Color(10, 133, 255),
		Color(10, 133, 255),
		Color(9, 133, 255),
		Color(9, 132, 255),
		Color(8, 132, 255),
		Color(8, 132, 255),
		Color(8, 132, 255),
		Color(7, 132, 255),
		Color(7, 131, 255),
		Color(6, 131, 255),
		Color(6, 131, 255),
		Color(5, 131, 255),
		Color(5, 130, 255),
		Color(4, 130, 255),
		Color(4, 130, 255),
		Color(4, 130, 255),
		Color(3, 130, 255),
		Color(3, 129, 255),
		Color(2, 129, 255),
		Color(2, 129, 255),
		Color(1, 129, 255),
		Color(1, 128, 255),
		Color(0, 128, 255),
		Color(0, 128, 255),
		Color(0, 128, 255),
		Color(0, 128, 255),
		Color(0, 128, 255),
		Color(0, 127, 255),
		Color(0, 127, 255),
		Color(0, 127, 255),
		Color(0, 127, 255),
		Color(0, 127, 255),
		Color(0, 127, 255),
		Color(0, 127, 255),
		Color(0, 126, 255),
		Color(0, 126, 255),
		Color(0, 126, 255),
		Color(0, 126, 255),
		Color(0, 126, 255),
		Color(0, 126, 255),
		Color(0, 126, 255),
		Color(0, 126, 255),
		Color(0, 126, 255),
		Color(0, 125, 255),
		Color(0, 125, 255),
		Color(0, 125, 255),
		Color(0, 125, 255),
		Color(0, 125, 255),
		Color(0, 125, 255),
		Color(0, 125, 255),
		Color(0, 124, 255),
		Color(0, 124, 255),
		Color(0, 124, 255),
		Color(0, 124, 255),
		Color(0, 124, 255),
		Color(0, 124, 255),
		Color(0, 124, 255),
		Color(0, 124, 255),
		Color(0, 124, 255),
		Color(0, 123, 255),
		Color(0, 123, 255),
		Color(0, 123, 255),
		Color(0, 123, 255),
		Color(0, 123, 255),
		Color(0, 123, 255),
		Color(0, 123, 255),
		Color(0, 122, 255),
		Color(0, 122, 255),
		Color(0, 122, 255),
		Color(0, 122, 255),
		Color(0, 122, 255),
		Color(0, 122, 255),
		Color(0, 122, 255),
		Color(0, 122, 255),
		Color(0, 122, 255),
		Color(0, 121, 255),
		Color(0, 121, 255),
		Color(0, 121, 255),
		Color(0, 121, 255),
		Color(0, 121, 255),
		Color(0, 121, 255),
		Color(0, 121, 255),
		Color(0, 120, 255),
		Color(0, 120, 255),
		Color(0, 120, 255),
		Color(0, 120, 255),
		Color(0, 120, 255),
		Color(0, 120, 255),
		Color(0, 120, 255),
		Color(0, 120, 255),
		Color(0, 120, 255),
		Color(0, 119, 255),
		Color(0, 119, 255),
		Color(0, 119, 255),
		Color(0, 119, 255),
		Color(0, 119, 255),
		Color(0, 119, 255),
		Color(0, 119, 255),
		Color(0, 118, 255),
		Color(0, 118, 255),
		Color(0, 118, 255),
		Color(0, 118, 255),
		Color(0, 118, 255),
		Color(0, 118, 255),
		Color(0, 118, 255),
		Color(0, 118, 255),
		Color(0, 118, 255),
		Color(0, 117, 255),
		Color(0, 117, 255),
		Color(0, 117, 255),
		Color(0, 117, 255),
		Color(0, 117, 255),
		Color(0, 117, 255),
		Color(0, 117, 255),
		Color(0, 116, 255),
		Color(0, 116, 255),
		Color(0, 116, 255),
		Color(0, 116, 255),
		Color(0, 116, 255),
		Color(0, 116, 255),
		Color(0, 116, 255),
		Color(0, 116, 255),
		Color(0, 116, 255),
		Color(0, 115, 255),
		Color(0, 115, 255),
		Color(0, 115, 255),
		Color(0, 115, 255),
		Color(0, 115, 255),
		Color(0, 115, 255),
		Color(0, 115, 255),
		Color(0, 114, 255),
		Color(0, 114, 255),
		Color(0, 114, 255),
		Color(0, 114, 255),
		Color(0, 114, 255),
		Color(0, 114, 255),
		Color(0, 114, 255),
		Color(0, 114, 255),
		Color(0, 114, 255),
		Color(0, 113, 255),
		Color(0, 113, 255),
		Color(0, 113, 255),
		Color(0, 113, 255),
		Color(0, 113, 255),
		Color(0, 113, 255),
		Color(0, 113, 255),
		Color(0, 112, 255),
		Color(0, 112, 255),
		Color(0, 112, 255),
		Color(0, 112, 255),
		Color(0, 112, 255),
		Color(0, 112, 255),
		Color(0, 112, 255),
		Color(0, 112, 255),
		Color(0, 112, 255),
		Color(0, 111, 255),
		Color(0, 111, 255),
		Color(0, 111, 255),
		Color(0, 111, 255),
		Color(0, 111, 255),
		Color(0, 111, 255),
		Color(0, 111, 255),
		Color(0, 110, 255),
		Color(0, 110, 255),
		Color(0, 110, 255),
		Color(0, 110, 255),
		Color(0, 110, 255),
		Color(0, 110, 255),
		Color(0, 110, 255),
		Color(0, 110, 255),
		Color(0, 110, 255),
		Color(0, 109, 255),
		Color(0, 109, 255),
		Color(0, 109, 255),
		Color(0, 109, 255),
		Color(0, 109, 255),
		Color(0, 109, 255),
		Color(0, 109, 255),
		Color(0, 108, 255),
		Color(0, 108, 255),
		Color(0, 108, 255),
		Color(0, 108, 255),
		Color(0, 108, 255),
		Color(0, 108, 255),
		Color(0, 108, 255),
		Color(0, 108, 255),
		Color(0, 108, 255),
		Color(0, 107, 255),
		Color(0, 107, 255),
		Color(0, 107, 255),
		Color(0, 107, 255),
		Color(0, 107, 255),
		Color(0, 107, 255),
		Color(0, 107, 255),
		Color(0, 106, 255),
		Color(0, 106, 255),
		Color(0, 106, 255),
		Color(0, 106, 255),
		Color(0, 106, 255),
		Color(0, 106, 255),
		Color(0, 106, 255),
		Color(0, 106, 255),
		Color(0, 106, 255),
		Color(0, 105, 255),
		Color(0, 105, 255),
		Color(0, 105, 255),
		Color(0, 105, 255),
		Color(0, 105, 255),
		Color(0, 105, 255),
		Color(0, 105, 255),
		Color(0, 104, 255),
		Color(0, 104, 255),
		Color(0, 104, 255),
		Color(0, 104, 255),
		Color(0, 104, 255),
		Color(0, 104, 255),
		Color(0, 104, 255),
		Color(0, 104, 255),
		Color(0, 104, 255),
		Color(0, 103, 255),
		Color(0, 103, 255),
		Color(0, 103, 255),
		Color(0, 103, 255),
		Color(0, 103, 255),
		Color(0, 103, 255),
		Color(0, 103, 255),
		Color(0, 102, 255),
		Color(0, 102, 255),
		Color(0, 102, 255),
		Color(0, 102, 255),
		Color(0, 102, 255),
		Color(0, 102, 255),
		Color(0, 102, 255),
		Color(0, 102, 255),
		Color(0, 102, 255),
		Color(0, 101, 255),
		Color(0, 101, 255),
		Color(0, 101, 255),
		Color(0, 101, 255),
		Color(0, 101, 255),
		Color(0, 101, 255),
		Color(0, 101, 255),
		Color(0, 100, 255),
		Color(0, 100, 255),
		Color(0, 100, 255),
		Color(0, 100, 255),
		Color(0, 100, 255),
		Color(0, 100, 255),
		Color(0, 100, 255),
		Color(0, 100, 255),
		Color(0, 100, 255),
		Color(0, 99, 255),
		Color(0, 99, 255),
		Color(0, 99, 255),
		Color(0, 99, 255),
		Color(0, 99, 255),
		Color(0, 99, 255),
		Color(0, 99, 255),
		Color(0, 98, 255),
		Color(0, 98, 255),
		Color(0, 98, 255),
		Color(0, 98, 255),
		Color(0, 98, 255),
		Color(0, 98, 255),
		Color(0, 98, 255),
		Color(0, 98, 255),
		Color(0, 98, 255),
		Color(0, 97, 255),
		Color(0, 97, 255),
		Color(0, 97, 255),
		Color(0, 97, 255),
		Color(0, 97, 255),
		Color(0, 97, 255),
		Color(0, 97, 255),
		Color(0, 96, 255),
		Color(0, 96, 255),
		Color(0, 96, 255),
		Color(0, 96, 255),
		Color(0, 96, 255),
		Color(0, 96, 255),
		Color(0, 96, 255),
		Color(0, 96, 255),
		Color(0, 96, 255),
		Color(0, 95, 255),
		Color(0, 95, 255),
		Color(0, 95, 255),
		Color(0, 95, 255),
		Color(0, 95, 255),
		Color(0, 95, 255),
		Color(0, 95, 255),
		Color(0, 94, 255),
		Color(0, 94, 255),
		Color(0, 94, 255),
		Color(0, 94, 255),
		Color(0, 94, 255),
		Color(0, 94, 255),
		Color(0, 94, 255),
		Color(0, 94, 255),
		Color(0, 94, 255),
		Color(0, 93, 255),
		Color(0, 93, 255),
		Color(0, 93, 255),
		Color(0, 93, 255),
		Color(0, 93, 255),
		Color(0, 93, 255),
		Color(0, 93, 255),
		Color(0, 92, 255),
		Color(0, 92, 255),
		Color(0, 92, 255),
		Color(0, 92, 255),
		Color(0, 92, 255),
		Color(0, 92, 255),
		Color(0, 92, 255),
		Color(0, 92, 255),
		Color(0, 92, 255),
		Color(0, 91, 255),
		Color(0, 91, 255),
		Color(0, 91, 255),
		Color(0, 91, 255),
		Color(0, 91, 255),
		Color(0, 91, 255),
		Color(0, 91, 255),
		Color(0, 90, 255),
		Color(0, 90, 255),
		Color(0, 90, 255),
		Color(0, 90, 255),
		Color(0, 90, 255),
		Color(0, 90, 255),
		Color(0, 90, 255),
		Color(0, 90, 255),
		Color(0, 90, 255),
		Color(0, 89, 255),
		Color(0, 89, 255),
		Color(0, 89, 255),
		Color(0, 89, 255),
		Color(0, 89, 255),
		Color(0, 89, 255),
		Color(0, 89, 255),
		Color(0, 88, 255),
		Color(0, 88, 255),
		Color(0, 88, 255),
		Color(0, 88, 255),
		Color(0, 88, 255),
		Color(0, 88, 255),
		Color(0, 88, 255),
		Color(0, 88, 255),
		Color(0, 88, 255),
		Color(0, 87, 255),
		Color(0, 87, 255),
		Color(0, 87, 255),
		Color(0, 87, 255),
		Color(0, 87, 255),
		Color(0, 87, 255),
		Color(0, 87, 255),
		Color(0, 86, 255),
		Color(0, 86, 255),
		Color(0, 86, 255),
		Color(0, 86, 255),
		Color(0, 86, 255),
		Color(0, 86, 255),
		Color(0, 86, 255),
		Color(0, 86, 255),
		Color(0, 86, 255),
		Color(0, 85, 255),
		Color(0, 85, 255),
		Color(0, 85, 255),
		Color(0, 85, 255),
		Color(0, 85, 255),
		Color(0, 85, 255),
		Color(0, 85, 255),
		Color(0, 84, 255),
		Color(0, 84, 255),
		Color(0, 84, 255),
		Color(0, 84, 255),
		Color(0, 84, 255),
		Color(0, 84, 255),
		Color(0, 84, 255),
		Color(0, 84, 255),
		Color(0, 84, 255),
		Color(0, 83, 255),
		Color(0, 83, 255),
		Color(0, 83, 255),
		Color(0, 83, 255),
		Color(0, 83, 255),
		Color(0, 83, 255),
		Color(0, 83, 255),
		Color(0, 82, 255),
		Color(0, 82, 255),
		Color(0, 82, 255),
		Color(0, 82, 255),
		Color(0, 82, 255),
		Color(0, 82, 255),
		Color(0, 82, 255),
		Color(0, 82, 255),
		Color(0, 82, 255),
		Color(0, 81, 255),
		Color(0, 81, 255),
		Color(0, 81, 255),
		Color(0, 81, 255),
		Color(0, 81, 255),
		Color(0, 81, 255),
		Color(0, 81, 255),
		Color(0, 80, 255),
		Color(0, 80, 255),
		Color(0, 80, 255),
		Color(0, 80, 255),
		Color(0, 80, 255),
		Color(0, 80, 255),
		Color(0, 80, 255),
		Color(0, 80, 255),
		Color(0, 80, 255),
		Color(0, 79, 255),
		Color(0, 79, 255),
		Color(0, 79, 255),
		Color(0, 79, 255),
		Color(0, 79, 255),
		Color(0, 79, 255),
		Color(0, 79, 255),
		Color(0, 78, 255),
		Color(0, 78, 255),
		Color(0, 78, 255),
		Color(0, 78, 255),
		Color(0, 78, 255),
		Color(0, 78, 255),
		Color(0, 78, 255),
		Color(0, 78, 255),
		Color(0, 78, 255),
		Color(0, 77, 255),
		Color(0, 77, 255),
		Color(0, 77, 255),
		Color(0, 77, 255),
		Color(0, 77, 255),
		Color(0, 77, 255),
		Color(0, 77, 255),
		Color(0, 76, 255),
		Color(0, 76, 255),
		Color(0, 76, 255),
		Color(0, 76, 255),
		Color(0, 76, 255),
		Color(0, 76, 255),
		Color(0, 76, 255),
		Color(0, 76, 255),
		Color(0, 76, 255),
		Color(0, 75, 255),
		Color(0, 75, 255),
		Color(0, 75, 255),
		Color(0, 75, 255),
		Color(0, 75, 255),
		Color(0, 75, 255),
		Color(0, 75, 255),
		Color(0, 74, 255),
		Color(0, 74, 255),
		Color(0, 74, 255),
		Color(0, 74, 255),
		Color(0, 74, 255),
		Color(0, 74, 255),
		Color(0, 74, 255),
		Color(0, 74, 255),
		Color(0, 74, 255),
		Color(0, 73, 255),
		Color(0, 73, 255),
		Color(0, 73, 255),
		Color(0, 73, 255),
		Color(0, 73, 255),
		Color(0, 73, 255),
		Color(0, 73, 255),
		Color(0, 72, 255),
		Color(0, 72, 255),
		Color(0, 72, 255),
		Color(0, 72, 255),
		Color(0, 72, 255),
		Color(0, 72, 255),
		Color(0, 72, 255),
		Color(0, 72, 255),
		Color(0, 72, 255),
		Color(0, 71, 255),
		Color(0, 71, 255),
		Color(0, 71, 255),
		Color(0, 71, 255),
		Color(0, 71, 255),
		Color(0, 71, 255),
		Color(0, 71, 255),
		Color(0, 70, 255),
		Color(0, 70, 255),
		Color(0, 70, 255),
		Color(0, 70, 255),
		Color(0, 70, 255),
		Color(0, 70, 255),
		Color(0, 70, 255),
		Color(0, 70, 255),
		Color(0, 70, 255),
		Color(0, 69, 255),
		Color(0, 69, 255),
		Color(0, 69, 255),
		Color(0, 69, 255),
		Color(0, 69, 255),
		Color(0, 69, 255),
		Color(0, 69, 255),
		Color(0, 68, 255),
		Color(0, 68, 255),
		Color(0, 68, 255),
		Color(0, 68, 255),
		Color(0, 68, 255),
		Color(0, 68, 255),
		Color(0, 68, 255),
		Color(0, 68, 255),
		Color(0, 68, 255),
		Color(0, 67, 255),
		Color(0, 67, 255),
		Color(0, 67, 255),
		Color(0, 67, 255),
		Color(0, 67, 255),
		Color(0, 67, 255),
		Color(0, 67, 255),
		Color(0, 66, 255),
		Color(0, 66, 255),
		Color(0, 66, 255),
		Color(0, 66, 255),
		Color(0, 66, 255),
		Color(0, 66, 255),
		Color(0, 66, 255),
		Color(0, 66, 255),
		Color(0, 66, 255),
		Color(0, 65, 255),
		Color(0, 65, 255),
		Color(0, 65, 255),
		Color(0, 65, 255),
		Color(0, 65, 255),
		Color(0, 65, 255),
		Color(0, 65, 255),
		Color(0, 64, 255),
		Color(0, 64, 255),
		Color(0, 64, 255),
		Color(0, 64, 255),
		Color(0, 64, 255)
	};
}
