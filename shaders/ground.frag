#version 150

in vec2 infragTexCoord;
out vec4 out_Color;

uniform sampler2D texUnit1;

void main(void)
{
	out_Color = texture(texUnit1, infragTexCoord);
}
