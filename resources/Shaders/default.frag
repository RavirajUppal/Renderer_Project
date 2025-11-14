#version 330 core
out vec4 FragColor;

in vec3 currPos;
in vec3 normal;
in vec3 color;
in vec2 texCoord;
in vec4 fragPosLightSpace;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D shadowMap;
uniform samplerCube shadowCubeMap;
uniform bool useShadow;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 CameraPos;
uniform int lightMode;
uniform float farPlane;

float ShadowCalculation(vec4 fragPosLight, vec3 normal, vec3 lightDirection)
{
	float shadow = 0.0f;
	vec3 lightCoords = fragPosLight.xyz / fragPosLight.w;
	if(lightCoords.z <= 1.0f)
	{
		lightCoords = lightCoords * 0.5 + 0.5;
		float currentDepth = lightCoords.z;
		float bias = max(0.025f * (1.0f - dot(normal, lightDirection)), 0.0005f);

		// Smoothens out the shadows
		int sampleRadius = 2;
		vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
		for(int y = -sampleRadius; y <= sampleRadius; y++)
		{
		    for(int x = -sampleRadius; x <= sampleRadius; x++)
		    {
		        float closestDepth = texture(shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
				if (currentDepth > closestDepth + bias)
					shadow += 1.0f;     
		    }    
		}
		// Get average shadow
		shadow /= pow((sampleRadius * 2 + 1), 2);
	}

    return shadow;
} 

float ShadowCalculationUsingCubeMap(vec3 normal, vec3 lightDirection){
	float shadow = 0.0f;
	vec3 fragToLight = currPos - lightPos;
	float currentDepth = length(fragToLight);
	float bias = max(0.5f * (1.0f - dot(normal, lightDirection)), 0.0005f); 

	// Not really a radius, more like half the width of a square
	int sampleRadius = 2;
	float offset = 0.02f;
	for(int z = -sampleRadius; z <= sampleRadius; z++)
	{
		for(int y = -sampleRadius; y <= sampleRadius; y++)
		{
		    for(int x = -sampleRadius; x <= sampleRadius; x++)
		    {
		        float closestDepth = texture(shadowCubeMap, fragToLight + vec3(x, y, z) * offset).r;
				// Remember that we divided by the farPlane?
				// Also notice how the currentDepth is not in the range [0, 1]
				closestDepth *= farPlane;
				if (currentDepth > closestDepth + bias)
					shadow += 1.0f;     
		    }    
		}
	}
	// Average shadow
	shadow /= pow((sampleRadius * 2 + 1), 3);
	return shadow;
}

vec4 DirectionalLight()
{
  float ambient = 0.2f;
  vec3 Normal = normalize(normal);
  vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
  float diffuse = max(dot(lightDirection, Normal), 0.0f);

  float specular = 0.0f;
  if (diffuse != 0.0f)
  {
  	float specularLight = 0.5f;
  	vec3 viewDirection = normalize(CameraPos - currPos);
  	vec3 reflectionDirection = reflect(-lightDirection, Normal);
  	float specAmount = pow(max(dot(reflectionDirection, viewDirection), 0.0f), 12);
  	specular = specAmount * specularLight;
  }

  float shadow = useShadow ? ShadowCalculation(fragPosLightSpace, Normal, lightDirection) : 0.0f;

  return (texture(diffuse0, texCoord)  * (diffuse * (1.0f - shadow) + ambient) + texture(specular0, texCoord).r  * specular * (1.0f - shadow)) * lightColor ;
}

vec4 PointLight()
{
  vec3 lightVec = lightPos - currPos;
  float dist = length(lightVec);
  float a = 0.1f;
  float b = 0.3f;
  float inten = 3.0f/(a * dist * dist + b * dist + 1.0f);
  float ambient = 0.2f;
  vec3 Normal = normalize(normal);
  vec3 lightDirection = normalize(lightVec);
  float diffuse = max(dot(lightDirection, Normal), 0.0f);

  float specular = 0.0f;
  if (diffuse != 0.0f)
  {
      float specularLight = 0.5f;
      vec3 viewDirection = normalize(CameraPos - currPos);
      vec3 reflectionDirection = reflect(-lightDirection, Normal);
      vec3 halfwayVec = normalize(lightDirection + viewDirection);

      float specAmount = pow(max(dot(halfwayVec, Normal), 0.0f), 12);
      specular = specAmount * specularLight;
  }

  float shadow = useShadow ? ShadowCalculationUsingCubeMap(Normal, lightDirection) : 0.0f;
	
  return (texture(diffuse0, texCoord)  * (diffuse * (1.0f - shadow) * inten + ambient) + texture(specular0, texCoord).r  * specular * (1.0f - shadow) * inten) * lightColor ;
}

vec4 SpotLight()
{
	// controls how big the area that is lit up is
	float outerCone = 0.90f;
	float innerCone = 0.95f;

	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 Normal = normalize(normal);
	vec3 lightDirection = normalize(lightPos - currPos);
	float diffuse = max(dot(Normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(CameraPos - currPos);
	vec3 reflectionDirection = reflect(-lightDirection, Normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	// calculates the intensity of the currPos based on its angle to the center of the light cone
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

  	float shadow = useShadow ? ShadowCalculation(fragPosLightSpace, Normal, lightDirection) : 0.0f;

	return (texture(diffuse0, texCoord) * (diffuse * (1.0f - shadow) * inten + ambient) + texture(specular0, texCoord).r * specular * (1.0f - shadow) * inten) * lightColor;
}

vec4 SelectLightMode()
{
  if (lightMode == 1)
  {
    return PointLight();
  }
  else if (lightMode == 2)
  {
    return SpotLight();
  }
  else
  {
    return DirectionalLight();
  }
}

void main()
{
  FragColor = SelectLightMode();
}

