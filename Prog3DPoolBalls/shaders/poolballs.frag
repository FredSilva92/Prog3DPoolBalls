#version 440 core

uniform mat4 Model;
uniform mat4 View;
uniform mat4 ModelView;		// View * Model
uniform int lightModel;
uniform sampler2D sampler;
uniform int renderTex;

// Estrutura da fonte de luz ambiente global
struct AmbientLight {
	vec3 ambient;	// Componente de luz ambiente global
};

uniform AmbientLight ambientLight; // Fonte de luz ambiente global

// Estrutura de uma fonte de luz direcional
struct DirectionalLight	{
	vec3 direction;		// Dire��o da luz, espa�o do mundo
	
	vec3 ambient;		// Componente de luz ambiente
	vec3 diffuse;		// Componente de luz difusa
	vec3 specular;		// Componente de luz especular
};

uniform DirectionalLight directionalLight; // Fonte de luz direcional

// Estrutura de uma fonte de luz pontual
struct PointLight	{
	vec3 position;		// Posi��o do ponto de luz, espa�o do mundo
	
	vec3 ambient;		// Componente de luz ambiente
	vec3 diffuse;		// Componente de luz difusa
	vec3 specular;		// Componente de luz especular
	
	float constant;		// Coeficiente de atenua��o constante
	float linear;		// Coeficiente de atenua��o linear
	float quadratic;	// Coeficiente de atenua��o quadr�tica
};

uniform PointLight pointLight; // Duas fontes de luz pontual

// Estrutura de uma fonte de luz c�nica
struct SpotLight {
	vec3 position;		// Posi��o do foco de luz, espa�o do mundo
	
	vec3 ambient;		// Componente de luz ambiente
	vec3 diffuse;		// Componente de luz difusa
	vec3 specular;		// Componente de luz especular
	
	float constant;		// Coeficiente de atenua��o constante
	float linear;		// Coeficiente de atenua��o linear
	float quadratic;	// Coeficiente de atenua��o quadr�tica

	float spotCutoff, spotExponent;
	vec3 spotDirection;
};

uniform SpotLight spotLight; // Fonte de luz c�nica

struct Material{
	vec3 emissive;
	vec3 ambient;		// Ka
	vec3 diffuse;		// kd
	vec3 specular;		// ke
	float shininess;
};

uniform Material material;

layout(location = 0) in vec3 color;
layout(location = 1) in vec2 textureCoord;

layout(location = 2) in vec3 vPositionEyeSpace;
layout(location = 3) in vec3 vNormalEyeSpace;
layout(location = 4) in vec3 textureVector;

layout (location = 0) out vec4 fColor; // Cor final do fragmento

vec4 calcAmbientLight(AmbientLight light);
vec4 calcDirectionalLight(DirectionalLight light);
vec4 calcPointLight(PointLight light);
//vec4 calcSpotLight(SpotLight light);

void main()
{
	// C�lculo da componente emissiva do material.
	vec4 emissive = vec4(material.emissive, 1.0);

	// C�lculo do efeito da ilumina��o no fragmento.
	vec4 light[5];
	// Contribui��o da fonte de luz ambiente
	light[0] = calcAmbientLight(ambientLight);
	// Contribui��o da fonte de luz direcional
	light[1] = calcDirectionalLight(directionalLight);

	light[2] = calcPointLight(pointLight);
	// Contribui��o de cada fonte de luz Pontual
	/*	for(int i=0; i<2; i++)
		light[i+2] = calcPointLight(pointLight[i]);
	// Contribui��o da fonte de luz c�nica
	light[4] = vec4(0.0);*/

	// C�lculo da cor final do fragmento.
	// Com CubeMap
	//fColor = (emissive + light[0] + light[1] + light[2] + light[3] + light[4]) * texture(cubeMap, textureVector);
	// Com cor de fragmento
	//fColor = (emissive + light[0] + light[1] + light[2] + light[3] + light[4]) * vec4(1.0, 0.5, 0.5, 1.0);
	/*
	if (lightModel == 1) {
		fColor = light[0] * vec4(color, 1.0f);
	} else {
		fColor = light[1] * vec4(color, 1.0f);
	}*/

	//fColor = vec4(color, 1.0f);

	if (renderTex == 1) {
		vec4 texColor = texture(sampler, textureCoord);
		fColor = texColor;
	} else {
		fColor = vec4(color, 1.0f);
	}

}

vec4 calcAmbientLight(AmbientLight light) {
	// C�lculo da contribui��o da fonte de luz ambiente global, para a cor do objeto.
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);
	return ambient;
}

vec4 calcDirectionalLight(DirectionalLight light) {
	// C�lculo da reflex�o da componente da luz ambiente.
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);

	// C�lculo da reflex�o da componente da luz difusa.
	vec3 lightDirectionEyeSpace = (View * vec4(light.direction, 0.0)).xyz;
	vec3 L = normalize(-lightDirectionEyeSpace); // Dire��o inversa � da dire��o luz.
	vec3 N = normalize(vNormalEyeSpace);
	float NdotL = max(dot(N, L), 0.0);
	vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;
	
	// C�lculo da reflex�o da componente da luz especular.
	// Como os c�lculos est�o a ser realizados nas coordenadas do olho, ent�o a c�mara est� na posi��o (0,0,0).
	// Resulta ent�o um vetor V entre os pontos (0,0,0) e vPositionEyeSpace:
	//		V = (0,0,0) - vPositionEyeSpace = (0-vPositionEyeSpace.x, 0-vPositionEyeSpace.y, 0-vPositionEyeSpace.z)
	// Que pode ser simplificado como:
	//		- vPositionEyeSpace
	vec3 V = normalize(-vPositionEyeSpace);
	//vec4 H = normalize(L + V);	// Modelo Blinn-Phong
	vec3 R = reflect(-L, N);
	float RdotV = max(dot(R, V), 0.0);
	//float NdotH = max(dot(N, H), 0.0);	// Modelo Blinn-Phong
	vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);

	// C�lculo da contribui��o da fonte de luz direcional para a cor final do fragmento.
	return (ambient + diffuse + specular);
}

vec4 calcPointLight(PointLight light) {
	// C�lculo da reflex�o da componente da luz ambiente.
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);

	// C�lculo da reflex�o da componente da luz difusa.
	//vec3 lightPositionEyeSpace = mat3(View) * light.position;
	vec3 lightPositionEyeSpace = (View * vec4(light.position, 1.0)).xyz;
	vec3 L = normalize(lightPositionEyeSpace - vPositionEyeSpace);
	vec3 N = normalize(vNormalEyeSpace);
	float NdotL = max(dot(N, L), 0.0);
	vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;

	// C�lculo da reflex�o da componente da luz especular.
	// Como os c�lculos est�o a ser realizados nas coordenadas do olho, ent�o a c�mara est� na posi��o (0,0,0).
	// Resulta ent�o um vetor V entre os pontos (0,0,0) e vPositionEyeSpace:
	//		V = (0,0,0) - vPositionEyeSpace = (0-vPositionEyeSpace.x, 0-vPositionEyeSpace.y, 0-vPositionEyeSpace.z)
	// Que pode ser simplificado como:
	//		- vPositionEyeSpace
	vec3 V = normalize(-vPositionEyeSpace);
	//vec4 H = normalize(L + V);	// Modelo Blinn-Phong
	vec3 R = reflect(-L, N);
	float RdotV = max(dot(R, V), 0.0);
	//float NdotH = max(dot(N, H), 0.0);	// Modelo Blinn-Phong
	vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);
	
	// attenuation
	float dist = length(mat3(View) * light.position - vPositionEyeSpace);	// C�lculo da dist�ncia entre o ponto de luz e o v�rtice
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	// C�lculo da contribui��o da fonte de luz pontual para a cor final do fragmento.
	return (attenuation * (ambient + diffuse + specular));
}

//vec4 calcSpotLight(SpotLight light) {}