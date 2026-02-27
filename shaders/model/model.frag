#version 460 core

in vec2 TexCoords;
in vec3 Normal;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

vec3 CalcDirLight(DirLight light, vec3 normal)
{
    vec3 light_dir = normalize(-light.direction);

    normal = normalize(normal);
    float diff = max(dot(normal, light_dir), 0.0);

    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));

    return (ambient + diffuse);
}

uniform DirLight dirLight;

void main()
{
    vec3 result = CalcDirLight(dirLight, Normal);
    //FragColor = texture(texture_diffuse1, TexCoords);
    FragColor = vec4(result, 1.0f);
}