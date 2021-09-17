#version 150

in vec2 infragTexCoord;
out vec4 out_Color;

uniform sampler2D texUnit2;

void main(void)
{

	out_Color = texture(texUnit2, infragTexCoord);
}
