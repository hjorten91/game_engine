#version 150

in vec3 in_frag_Normal;
in vec2 infragTexCoord;
out vec4 out_Color;

uniform sampler2D texUnit;

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade;

	shade = dot(normalize(in_frag_Normal), light);
	//out_Color = vec4(shade, shade, shade, 1.0);

	out_Color = shade * texture(texUnit, infragTexCoord);
}
