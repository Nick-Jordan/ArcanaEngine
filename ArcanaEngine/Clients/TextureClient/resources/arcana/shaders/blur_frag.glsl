#version 400

layout(location = 0) out vec4 fs_FragColor;

in vec2 fs_TexCoord;

uniform sampler2D u_EmissiveTexture;

uniform bool horizontal;
uniform float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main()
{             
     vec2 tex_offset = 1.0 / textureSize(u_EmissiveTexture, 0); // gets size of single texel
     vec3 result = texture(u_EmissiveTexture, fs_TexCoord).rgb * weight[0];
     if(horizontal)
     {
         for(int i = 1; i < 5; ++i)
         {
            result += texture(u_EmissiveTexture, fs_TexCoord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(u_EmissiveTexture, fs_TexCoord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
         }
     }
     else
     {
         for(int i = 1; i < 5; ++i)
         {
             result += texture(u_EmissiveTexture, fs_TexCoord + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
             result += texture(u_EmissiveTexture, fs_TexCoord - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
         }
     }
     fs_FragColor = vec4(result, 1.0);
}