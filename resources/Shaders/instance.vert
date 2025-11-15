#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTex;
layout (location = 4) in mat4 aInstanceMatrix;

uniform mat4 cameraMatrix;

#ifdef USE_GEOMETRY
out DATA {
	vec3 currPos;
    vec3 normal;
    vec3 color;
    vec2 texCoord;
    mat4 projection;
} data_out;
#else
out vec3 currPos;
out vec3 normal;
out vec3 color;
out vec2 texCoord;
out vec4 fragPosLightSpace;
#endif

void main()
{
#ifdef USE_GEOMETRY
	data_out.currPos = vec3(aInstanceMatrix * vec4(aPos, 1.0f));
	data_out.normal = mat3(transpose(inverse(aInstanceMatrix))) * aNormal;
	data_out.color = aColor;
	data_out.texCoord = aTex;
	data_out.projection = cameraMatrix;
	gl_Position = aInstanceMatrix * vec4(aPos, 1.0f);
#else
	currPos = vec3(aInstanceMatrix * vec4(aPos, 1.0f));
	normal = mat3(transpose(inverse(aInstanceMatrix))) * aNormal;
	color = aColor;
	texCoord = aTex;
	fragPosLightSpace = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	gl_Position = cameraMatrix * vec4(currPos, 1.0f);
#endif
}