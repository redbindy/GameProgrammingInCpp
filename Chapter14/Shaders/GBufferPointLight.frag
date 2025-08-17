#version 460

struct PointLight
{
	vec3 worldPos;
	vec3 diffuseColor;

	float innerRadius;
	float outerRadius;
};

uniform sampler2D uGDiffuse;
uniform sampler2D uGNormal;
uniform sampler2D uGWorldPos;

uniform PointLight uPointLight;
uniform vec2 uScreenDimensions;

in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main()
{
	vec2 gbufferCoord = gl_FragCoord.xy / uScreenDimensions;

	vec3 gbufferDiffuse = texture(uGDiffuse, gbufferCoord).xyz;
	vec3 gbufferNorm = texture(uGNormal, gbufferCoord).xyz;
	vec3 gbufferWorldPos = texture(uGWorldPos, gbufferCoord).xyz;

	vec3 n = normalize(gbufferNorm);
	vec3 l = normalize(uPointLight.worldPos - gbufferWorldPos);

	vec3 phong = vec3(0.0, 0.0, 0.0);
	float nDotL = dot(n, l);
	if (nDotL > 0)
	{
		float dist = distance(uPointLight.worldPos, gbufferWorldPos);
		float intensity = smoothstep(uPointLight.innerRadius, uPointLight.outerRadius, dist);

		vec3 diffuseColor = mix(uPointLight.diffuseColor, vec3(0.0, 0.0, 0.0), intensity);

		phong = diffuseColor * nDotL;
	}

	outColor = vec4(gbufferDiffuse * phong, 1.0);
}