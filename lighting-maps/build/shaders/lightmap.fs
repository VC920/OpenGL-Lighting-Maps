#version 330 core
out vec4 FragColor;

// 材质
struct Material {
    sampler2D diffuse;  // 漫反射贴图（环境光贴图 == 漫反射贴图）
    sampler2D specular; // 镜面光贴图
    float shininess;    // 反光度
};

// 定向光
struct DirLight {
    vec3 direction; // 定向光方向

    vec3 ambient;   // 环境光强度
    vec3 diffuse;   // 漫反射强度
    vec3 specular;  // 静面光强度
};

// 点光源
struct PointLight {
    vec3 position;   // 点光源位置

    vec3 ambient;    // 环境光强度
    vec3 diffuse;    // 漫反射强度
    vec3 specular;   // 镜面光强度

    float constant;  // 衰减公式常数项
    float linear;    // 衰减公式一次项
    float quadratic; // 衰减公式二次项
};

// 聚光灯
struct SpotLight {
    vec3 position;      // 聚光灯位置
    vec3 direction;     // 聚光灯方向
    float cutOff;       // 聚光灯内切光角
    float outerCutOff;  // 聚光灯外切光角

    vec3 ambient;       // 环境光强度
    vec3 diffuse;       // 漫反射强度
    vec3 specular;      // 镜面光强度

    float constant;     // 衰减公式常数项
    float linear;       // 衰减公式一次项
    float quadratic;    // 衰减公式二次项
};

in vec3 Normal;     // 法线
in vec3 FragPos;    // 片段位置
in vec2 TexCoords;  // 纹理坐标

uniform vec3 viewPos;       // 观察位置
uniform Material material;  // 材质
uniform DirLight dirLight;  // 定向光
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS]; // 点光源们
uniform SpotLight spotLight;    // 聚光灯

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    // 属性
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // 第一阶段：定向光照
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // 第二阶段：点光源
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    // 第三阶段：聚光
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0f);
}

// 计算定向光
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0f);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    // 合并结果
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
    return (ambient + diffuse + specular);
}

// 计算点光源
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0f);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    // 衰减
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // 合并结果
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

// 计算聚光灯
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0f);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    // 计算聚光区域
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);
    // 衰减
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // 合并结果
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}