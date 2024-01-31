#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_roughness1;

uniform float material_shininess;

uniform vec3 ambientLightColor;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main() {
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 ambientColor = ambientLightColor * texture(texture_diffuse1, TexCoords).rgb;

    float diffuse = max(dot(normal, lightDir), 0.0);
    vec3 diffuseColor = diffuse * lightColor * texture(texture_diffuse1, TexCoords).rgb;

    float exponent = texture(texture_roughness1, TexCoords).r * material_shininess;

    vec3 specular = pow(max(dot(reflectDir, viewDir), 0.0), exponent) * lightColor;
    vec3 specularColor = texture(texture_specular1, TexCoords).r * specular;

    vec3 result = (ambientColor + diffuseColor + specularColor);
    FragColor = vec4(result, 1.0);
}