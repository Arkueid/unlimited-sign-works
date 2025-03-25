#version 330 core

in vec3 Normal;         
in vec3 FragPos;
in vec2 TexCoord;

uniform sampler2D tex;
// 平行光参数
uniform vec3 lightPos = vec3(0, 0, 3000);
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);  // 光颜色
uniform vec3 objectColor = vec3(0.5, 0.5, 0.5); // 物体基础颜色

// 材质参数
uniform vec3 ambientColor = vec3(0.2);  // 环境光强度

void main()
{
    vec3 texColor = texture(tex, TexCoord).rgb;
    
    // 环境光计算
    vec3 ambient = ambientColor * objectColor;
    
    // 漫反射计算
    vec3 norm = normalize(Normal);
    vec3 lightDirNorm = normalize(vec3(lightPos - FragPos));  // 指向光源方向
    float diff = max(dot(norm, lightDirNorm), 0.0);
    vec3 diffuse = diff * lightColor * objectColor;

    // 最终颜色合成
    vec3 result = ambient + diffuse;
    gl_FragColor = vec4(result, 1.0);
}