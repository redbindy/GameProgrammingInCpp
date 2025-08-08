#version 460

struct DirectionalLight
{
	vec3 direction;
	vec3 diffuseColor;
	vec3 specColor;
};

uniform vec3 uCameraPos;
uniform vec3 uAmbientLight;
uniform float uSpecPower;

uniform DirectionalLight uDirLight;

uniform sampler2D uTexture;

in vec3 fragWorldPos;
in vec3 fragNormal;
in vec2 fragTexCoord;

out vec4 outColor;

void main()
{
	vec3 n = normalize(fragNormal);
	vec3 l = normalize(-uDirLight.direction);
	vec3 v = normalize(uCameraPos - fragWorldPos);
	vec3 r = normalize(reflect(-l, n));

	vec3 phong = uAmbientLight;
	float nDotL = dot(n, l);
	if (nDotL > 0)
	{
		vec3 diffuse = uDirLight.diffuseColor * nDotL;
		vec3 specular = uDirLight.specColor * pow(max(0.0, dot(r, v)), uSpecPower);

		phong += diffuse + specular;
	}

	outColor = texture(uTexture, fragTexCoord) * vec4(phong, 1.f);
}