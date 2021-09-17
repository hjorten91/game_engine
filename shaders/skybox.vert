#version 150

in vec3 in_Position;
in vec2 inTexCoord;

out vec2 infragTexCoord;

uniform mat4 skyboxMatrix;

void main(void)
{

	gl_Position = skyboxMatrix * vec4(in_Position, 1.0);
	infragTexCoord = inTexCoord;
}
