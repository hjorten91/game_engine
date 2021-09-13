#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 inTexCoord;
in vec3 in_Ground;

out vec3 in_frag_Normal;
out vec2 infragTexCoord;

uniform mat4 totalMatrix;

void main(void)
{
	mat3 normalMatrix = mat3(totalMatrix);

	gl_Position = totalMatrix * vec4(in_Position, 1.0);
	in_frag_Normal = normalMatrix * in_Normal;
	infragTexCoord = inTexCoord;
}
