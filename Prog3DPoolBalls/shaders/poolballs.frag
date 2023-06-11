#version 440 core

uniform mat4 Model;
uniform mat4 View;
uniform mat4 ModelView;		
uniform mat3 NormalMatrix;
uniform int lightModel;
uniform sampler2D sampler;
uniform int renderTex;
uniform vec3 viewPosition;


struct AmbientLight {
	vec3 ambient;	
};

uniform AmbientLight ambientLight;	


struct DirectionalLight	{
	vec3 direction;		
	
	vec3 ambient;		
	vec3 diffuse;		
	vec3 specular;		
};

uniform DirectionalLight directionalLight;	


struct PointLight {
	vec3 position;		
	
	vec3 ambient;		
	vec3 diffuse;		
	vec3 specular;		
	
	float constant;		
	float linear;		
	float quadratic;	
};

uniform PointLight pointLight;	


struct SpotLight {
	vec3 position;		
	vec3 direction;		

	vec3 ambient;		
	vec3 diffuse;		
	vec3 specular;		
	
	float constant;		
	float linear;		
	float quadratic;	

	float cutoff, outerCutOff;
};

uniform SpotLight spotLight;

struct Material {
	vec3 emissive;
	vec3 ambient;		
	vec3 diffuse;		
	vec3 specular;		
	float shininess;
};

uniform Material material;

layout(location = 0) in vec3 color;
layout(location = 1) in vec2 textureCoord;

layout(location = 2) in vec3 vPositionEyeSpace;
layout(location = 3) in vec3 vNormalEyeSpace;
layout(location = 4) in vec3 textureVector;
layout(location = 5) in vec3 fragPos;

layout (location = 0) out vec4 fColor;

vec4 calcAmbientLight(AmbientLight light);
vec4 calcDirectionalLight(DirectionalLight light);
vec4 calcPointLight(PointLight light);
vec4 calcSpotLight(SpotLight light);

void main()
{
	
	vec4 emissive = vec4(material.emissive, 1.0);

	vec4 lightToUse;

	if (lightModel == 2) {
		lightToUse = calcDirectionalLight(directionalLight);
	} else if (lightModel == 3) {
		lightToUse = calcPointLight(pointLight);
	} else if (lightModel == 4) {
		lightToUse = calcSpotLight(spotLight);
	}  else {
		lightToUse = calcAmbientLight(ambientLight);
	}

	if (renderTex == 1) {
		vec4 texColor = texture(sampler, textureCoord);
		fColor = (emissive + lightToUse) * texColor;
	} else  {
		fColor = (emissive + lightToUse) * vec4(color, 1.0f);
	}
}

vec4 calcAmbientLight(AmbientLight light) {

	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);
	return ambient;
}

vec4 calcDirectionalLight(DirectionalLight light) {
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);

	vec3 lightDirectionEyeSpace = (View * vec4(light.direction, 0.0)).xyz;
	vec3 L = normalize(-lightDirectionEyeSpace);
	vec3 N = normalize(vNormalEyeSpace);
	float NdotL = max(dot(N, L), 0.0);
	vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;







	vec3 V = normalize(-vPositionEyeSpace);

	vec3 R = reflect(-L, N);
	float RdotV = max(dot(R, V), 0.0);

	vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);

	return (ambient + diffuse + specular);
}

vec4 calcPointLight(PointLight light) {
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);

	vec3 lightPositionEyeSpace = (View * vec4(light.position, 1.0)).xyz;
	vec3 L = normalize(lightPositionEyeSpace - vPositionEyeSpace);
	vec3 N = normalize(vNormalEyeSpace);
	float NdotL = max(dot(N, L), 0.0);
	vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;

	vec3 V = normalize(-vPositionEyeSpace);

	vec3 R = reflect(-L, N);
	float RdotV = max(dot(R, V), 0.0);
	
	vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);
	
	float dist = length(mat3(View) * light.position - vPositionEyeSpace);	
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	return (attenuation * (ambient + diffuse + specular));
}

vec4 calcSpotLight(SpotLight light) {

    vec3 ambient =  material.ambient * light.ambient;

    vec3 norm = normalize(color);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff;
    

    vec3 viewDir = normalize(viewPosition - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec;

	float theta = dot(lightDir, normalize(-light.direction));


	float epsilon = (light.cutoff - light.outerCutOff);
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	diffuse  *= intensity;
	specular *= intensity;

	float distance    = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;

	return vec4(ambient + diffuse + specular, 1.0f);
	
}