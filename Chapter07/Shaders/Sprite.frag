#version 460

uniform sampler2D uTexture;

in vec2 fragTexCoord;

out vec4 outColor;

void main()
{
	outColor = texture(uTexture, fragTexCoord);
}