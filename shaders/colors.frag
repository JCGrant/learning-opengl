#version 330 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct BaseLight {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct DirectionLight {
  BaseLight base;
  vec3 direction;
};

struct PointLight {
  BaseLight base;
  vec3 position;
  float constant;
  float linear;
  float quadratic;
};

struct SpotLight {
  PointLight point;
  vec3 direction;
  float cutOff;
  float outerCutOff;
};

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
#define NUM_DIRECTION_LIGHTS 1
uniform DirectionLight directionLights[NUM_DIRECTION_LIGHTS];
#define NUM_POINT_LIGHTS 4
uniform PointLight pointLights[NUM_POINT_LIGHTS];
#define NUM_SPOT_LIGHTS 1
uniform SpotLight spotLights[NUM_SPOT_LIGHTS];

vec3 calcBaseLight(BaseLight light, vec3 normal, vec3 viewDir, vec3 lightDir) {
  // diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);
  // specular shading
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  // combine results
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
  vec3 diffuse =
      light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
  vec3 specular =
      light.specular * spec * vec3(texture(material.specular, TexCoords));
  return ambient + diffuse + specular;
}

vec3 calcDirectionLight(DirectionLight light, vec3 normal, vec3 viewDir) {
  vec3 lightDir = normalize(-light.direction);
  vec3 baseLight = calcBaseLight(light.base, normal, viewDir, lightDir);
  return baseLight;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos) {
  vec3 lightDir = normalize(light.position - fragPos);
  vec3 baseLight = calcBaseLight(light.base, normal, viewDir, lightDir);
  // attenuation
  float distance = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance +
                             light.quadratic * (distance * distance));
  return attenuation * baseLight;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos) {
  vec3 pointLight = calcPointLight(light.point, normal, viewDir, fragPos);
  // spotlight intensity
  vec3 lightDir = normalize(light.point.position - fragPos);
  float theta = dot(lightDir, normalize(-light.direction));
  float epsilon = light.cutOff - light.outerCutOff;
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
  return intensity * pointLight;
}

void main() {
  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(viewPos - FragPos);

  vec3 result;
  for (int i = 0; i < NUM_DIRECTION_LIGHTS; i++) {
    result += calcDirectionLight(directionLights[i], norm, viewDir);
  }
  for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
    result += calcPointLight(pointLights[i], norm, viewDir, FragPos);
  }
  for (int i = 0; i < NUM_SPOT_LIGHTS; i++) {
    result += calcSpotLight(spotLights[i], norm, viewDir, FragPos);
  }

  FragColor = vec4(result, 1.0);
}
