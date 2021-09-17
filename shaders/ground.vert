#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 inTexCoord;

out vec3 in_frag_Normal;
out vec2 infragTexCoord;

uniform mat4 groundMatrix;

void main(void)
{
	mat3 normalMatrix = mat3(groundMatrix);

	gl_Position = groundMatrix * vec4(in_Position, 1.0);
	in_frag_Normal = normalMatrix * in_Normal;
	infragTexCoord = inTexCoord;
}
