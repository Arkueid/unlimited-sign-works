#version 450

layout (location = 0) in vec3 VertexPos;

uniform mat4 MVP;
//uniform mat4 ModelM;

void main()
{
    // vec3 FragPos = vec3(ModelM * vec4(VertexPos, 1.0));
     gl_Position = MVP * vec4(VertexPos.y, VertexPos.z, VertexPos.x, 1.0);//ProjectionM*  vec4(FragPos, 1.0);
}
