#version 400

layout(location = 0) out vec4 fs_FragColor;

in vec2 fs_TexCoord;

uniform sampler2D u_Texture;
uniform sampler2D u_ColorGradingLUT;

vec4 lookup(vec4 textureColor)
{
    #ifndef LUT_NO_CLAMP
        textureColor = clamp(textureColor, 0.0, 1.0);
    #endif

    float blueColor = textureColor.b * 63.0;

    vec2 quad1;
    quad1.y = floor(floor(blueColor) / 8.0);
    quad1.x = floor(blueColor) - (quad1.y * 8.0);

    vec2 quad2;
    quad2.y = floor(ceil(blueColor) / 8.0);
    quad2.x = ceil(blueColor) - (quad2.y * 8.0);

    vec2 texPos1;
    texPos1.x = (quad1.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.r);
    texPos1.y = (quad1.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.g);
 
    //flip texPos1.y = 1.0-texPos1.y;

    vec2 texPos2;
    texPos2.x = (quad2.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.r);
    texPos2.y = (quad2.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.g);

    //flip texPos2.y = 1.0-texPos2.y;

    vec4 newColor1 = texture(u_ColorGradingLUT, texPos1);
    vec4 newColor2 = texture(u_ColorGradingLUT, texPos2);

	return mix(newColor1, newColor2, fract(blueColor));
}


void main()
{
	vec4 color = texture(u_Texture, fs_TexCoord);
	vec4 result = lookup(color);

	fs_FragColor = result;
}