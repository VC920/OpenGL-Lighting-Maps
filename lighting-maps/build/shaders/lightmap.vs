#version 330 core
layout (location = 0) in vec3 aPos;         // 顶点信息
layout (location = 1) in vec3 aNormal;      // 法线信息
layout (location = 2) in vec2 aTexCoords;   // 纹理坐标信息

out vec3 Normal;    // 法线
out vec3 FragPos;   // 片段位置
out vec2 TexCoords; // 纹理坐标

uniform mat4 mvp;           // model + view + projection 矩阵 
uniform mat4 model;         // model矩阵（用于进行世界空间计算）
uniform mat3 normalMatrix;  // 法线矩阵（在CPU计算更快）

void main()
{
    Normal = normalMatrix * aNormal;            // 计算世界空间法线
    FragPos = vec3(model * vec4(aPos, 1.0f));   // 计算世界空间片段位置
    TexCoords = aTexCoords;                     // 传递纹理信息

    gl_Position = mvp * vec4(aPos, 1.0f);       // 应用顶点
}