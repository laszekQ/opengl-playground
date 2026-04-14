#version 460 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 fragPos;

out vec4 FragColor;

uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material{
    vec3 ambient;
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_emission;
    float shininess;
};
uniform Material material;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 light_dir = normalize(-light.direction);

    normal = normalize(normal);
    float diff = max(dot(normal, light_dir), 0.0);

    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(viewDir, reflect_dir), 0.0), material.shininess);

    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));

    return (ambient + diffuse + specular);
}

uniform DirLight dirLight;

uniform bool depthTesting;
float near = 0.1;
float far = 100.0;

void main()
{
    if(depthTesting)
    {
        // depth linearization
        float depth = gl_FragCoord.z;
        float z = depth * 2.0 - 1.0;
        vec3 result = vec3((2.0 * near * far) / (far + near - z * (far - near)));
        FragColor = vec4(result, 1.0f);
        return;
    }

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 result = CalcDirLight(dirLight, Normal, viewDir);
    //FragColor = texture(texture_diffuse1, TexCoords);
    FragColor = vec4(result, 1.0f);
}