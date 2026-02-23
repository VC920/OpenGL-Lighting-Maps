#version 330 core
out vec4 FragColor;

// 材质
struct Material {
    sampler2D diffuse;  // 漫反射贴图
    sampler2D specular; // 镜面光贴图
    float shininess;    // 反光度
};

// 光源
struct Light {
    // vec3 position;  // 光源位置
    vec3 direction; // 平行光方向
    vec3 ambient;   // 环境光强度
    vec3 diffuse;   // 漫反射光强度
    vec3 specular;  // 镜面光强度
};

in vec3 Normal;     // 法线
in vec3 FragPos;    // 片段位置
in vec2 TexCoords;  // 纹理位置

uniform vec3 viewPos;       // 观察位置
uniform Material material;  // 材质
uniform Light light;        // 光照

void main()
{
    // 环境光照
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;  // 计算环境光照（环境光照等于漫反射光照）

    // 漫反射光照
    vec3 norm = normalize(Normal);                                                  // 计算法线方向
    // vec3 lightDir = normalize(light.position - FragPos);                            // 计算照射方向
    vec3 lightDir = normalize(-light.direction);                            // 计算照射方向
    float diff = max(dot(norm, lightDir), 0.0f);                                    // 计算漫反射影响
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb; // 计算漫反射光照

    // 镜面光照
    vec3 viewDir = normalize(viewPos - FragPos);                                        // 计算观察方向
    vec3 reflectDir = reflect(-lightDir, norm);                                         // 计算反射方向
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);          // 计算反射影响
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  // 计算反射光照

    // 最终光照
    vec3 result = ambient + diffuse + specular; // 计算最终光照
    FragColor = vec4(result, 1.0f);             // 应用最终光线
}