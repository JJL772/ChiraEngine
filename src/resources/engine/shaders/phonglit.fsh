struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
    float lambertFactor; // between 0 and 1
};

struct PointLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
};

struct SpotLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    float cutoffOuter;
    float cutoffInner;
    float cutoffDegree;
};

in VS_OUT {
    vec3 Normal;
    vec3 Color;
    vec2 TexCoord;
    vec3 WorldPosition;
} i;

out vec4 FragColor;

#define MAX_POINT_LIGHTS 1//`MAX_POINT_LIGHTS`//
#define MAX_DIRECTIONAL_LIGHTS 1//`MAX_DIRECTIONAL_LIGHTS`//
#define MAX_SPOT_LIGHTS 1//`MAX_SPOT_LIGHTS`//

uniform Material material;
uniform PointLight pointLight[MAX_POINT_LIGHTS];
uniform DirectionalLight directionalLight[MAX_DIRECTIONAL_LIGHTS];
uniform SpotLight spotLight[MAX_SPOT_LIGHTS];

vec3 applyPointLight(PointLight light, vec3 normal, vec3 viewDirection) {
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, i.TexCoord));

    vec3 lightDirection = normalize(light.position - i.WorldPosition);
    float diff = max(dot(normal, lightDirection), 0.0) * material.lambertFactor + (1.0 - material.lambertFactor);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, i.TexCoord));

    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, i.TexCoord));

    float distance = length(light.position - i.WorldPosition);
    float attenuation = 1.0 / (light.constant + (light.linear * distance) + light.quadratic * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 applyDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection) {
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, i.TexCoord));

    vec3 lightDirection = normalize(-light.direction);
    float diff = max(dot(normal, lightDirection), 0.0) * material.lambertFactor + (1.0 - material.lambertFactor);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, i.TexCoord));

    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, i.TexCoord));

    return (ambient + diffuse + specular);
}

vec3 applySpotLight(SpotLight light, vec3 normal, vec3 viewDirection) {
    return vec3(0.0);
}

void main() {
    vec3 normal = normalize(i.Normal);
    vec3 viewDirection = normalize(-i.WorldPosition);

    vec3 result = vec3(texture(material.diffuse, i.TexCoord));
    //for (int i = 0; i < MAX_POINT_LIGHTS; i++)
    //    result += applyPointLight(pointLight[i], normal, viewDirection);
    for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS; i++)
        result += applyDirectionalLight(directionalLight[i], normal, viewDirection);
    //for (int i = 0; i < MAX_SPOT_LIGHTS; i++)
    //    result += applySpotLight(spotLight[i], normal, viewDirection);

    FragColor = vec4(i.Color, 1.0) * vec4(result, 1.0);
}
