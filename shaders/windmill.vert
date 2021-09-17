#version 150

in vec3 in_Position;
in vec2 inTexCoord;
in vec3 in_Normal;

out vec3 in_frag_Normal;
out vec2 infragTexCoord;

uniform mat4 windmillMatrix;

void main(void)
{
	//mat3 normalMatrix = mat3(windmillMatrix);


	gl_Position = windmillMatrix * vec4(in_Position, 1.0);
	infragTexCoord = inTexCoord;
	in_frag_Normal = in_Normal;
}
