#version 460

struct DirectionalLight
{
	vec3 direction;
	vec3 diffuseColor;
	vec3 specColor;
};

uniform sampler2D uGDiffuse;
uniform sampler2D uGNormal;
uniform sampler2D uGWorldPos;

uniform vec3 uCameraPos;
uniform vec3 uAmbientLight;
uniform DirectionalLight uDirLight;

in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main()
{
	vec3 gbufferDiffuse = texture(uGDiffuse, fragTexCoord).xyz;
	vec3 gbufferNorm = texture(uGNormal, fragTexCoord).xyz;
	vec3 gbufferWorldPos = texture(uGWorldPos, fragTexCoord).xyz;

	vec3 n = normalize(gbufferNorm);
	vec3 l = normalize(-uDirLight.direction);
	vec3 v = normalize(uCameraPos - gbufferWorldPos);
	vec3 r = normalize(reflect(-l, n));

	vec3 phong = uAmbientLight;
	float nDotL = dot(n, l);
	if (nDotL > 0)
	{
		vec3 diffuse = uDirLight.diffuseColor * nDotL;
		phong += diffuse;
	}

	phong = clamp(phong, 0.0, 1.0);

	outColor = vec4(gbufferDiffuse * phong, 1.0);
}