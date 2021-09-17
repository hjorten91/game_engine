#version 150

in vec3 in_frag_Normal;
in vec2 infragTexCoord;
out vec4 out_Color;

uniform sampler2D texUnit0;

//uniform vec3 lightSourcesDirPosArr[4];
//uniform vec3 lightSourcesColorArr[4];
//uniform float specularExponent;
//uniform bool isDirectional[4];

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float	shade = dot(normalize(in_frag_Normal), light);

	out_Color = shade * texture(texUnit0, infragTexCoord);
}
