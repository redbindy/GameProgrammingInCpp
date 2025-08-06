#version 460

in vec2 fragTexCoord;
in vec4 vertexColor;

out vec4 outColor;

uniform sampler2D uTexture;

void main()
{
	const vec4 textureColor = texture(uTexture, fragTexCoord);
	outColor = (textureColor + vertexColor) * 0.5f;
}