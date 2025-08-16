#version 460

in vec3 fragWorldPos;
in vec3 fragNormal;
in vec2 fragTexCoord;

layout(location = 0) out vec3 outDiffuse;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec3 outWorldPos;

uniform sampler2D uTexture;

void main()
{
	outDiffuse = texture(uTexture, fragTexCoord).xyz;
	outNormal = fragNormal;
	outWorldPos = fragWorldPos;
}