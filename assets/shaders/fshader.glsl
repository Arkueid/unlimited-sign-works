#version 330 core

in vec3 Normal;         
in vec3 FragPos;
in vec2 TexCoord;

uniform sampler2D tex;
// 平行光参数
uniform vec3 lightPos = vec3(1000, 1000, 3000);
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);  // 光颜色
uniform vec3 objectColor = vec3(0.8, 0.8, 0.8); // 物体基础颜色
uniform vec3 cameraPos = vec3(0.0, 0.0, 0.0); // 相机位置

// 材质参数
uniform vec3 ambientColor = vec3(0.2);  // 环境光强度

void main()
{
    vec4 col = texture(tex, TexCoord);

    vec3 texColor = col.rgb * col.a;
    
    // 环境光计算
    vec3 ambient = ambientColor * texColor;
    
    // 漫反射计算
    vec3 norm = normalize(Normal);
    vec3 lightDirNorm = normalize(vec3(lightPos - FragPos));  // 指向光源方向
    float diff = max(dot(norm, lightDirNorm), 0.0);
    vec3 diffuse = diff * lightColor * texColor;

    // 镜面光计算
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDirNorm, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = spec * lightColor * texColor;

    // 最终颜色合成
    vec3 result = ambient + diffuse + specular;
    gl_FragColor = vec4(result, col.a);
}