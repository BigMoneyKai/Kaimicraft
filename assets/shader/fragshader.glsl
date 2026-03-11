#version 330

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D uTexture1;
uniform vec3 viewPos;
uniform float shininess;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float kc;
    float kl;
    float kq;
};

#define MAX_POINT_LIGHTS 8
uniform int numPointLights;
uniform DirLight dirLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    vec3 diffuse = light.diffuse * max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 spec = light.specular * pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    return light.ambient + diffuse + spec;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 diffuse = light.diffuse * max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 spec = light.specular * pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.kc + light.kl * dist + light.kq * dist * dist);
    return (light.ambient + diffuse + spec) * attenuation;
}

void main(void) {
    vec3 normDir = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 lighting = CalcDirLight(dirLight, normDir, viewDir);
    int count = min(numPointLights, MAX_POINT_LIGHTS);
    for(int i = 0; i < count; i++) {
        lighting += CalcPointLight(pointLights[i], normDir, FragPos, viewDir);
    }
    
    // Texture
    vec3 textureColor = texture(uTexture1, TexCoord).rgb;
    
    // Final output color
    FragColor = vec4(textureColor * lighting, 1.0);
}
