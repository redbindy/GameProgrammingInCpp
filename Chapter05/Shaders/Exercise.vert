#version 460

uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 inVertexColor;

out vec2 fragTexCoord;
out vec4 vertexColor;

void main()
{
	vec4 pos = vec4(inPosition, 1.0);

	gl_Position = pos * uWorldTransform * uViewProj;
	fragTexCoord = inTexCoord;
	vertexColor = vec4(inVertexColor, 1.0);
}