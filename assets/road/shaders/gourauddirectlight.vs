#version 330 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 Color;

uniform mat4 ProjectionMatrix;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;

uniform vec3 lightColor;
uniform vec3 lightDirection;

uniform vec3 Ka; // Ambient reflectivity
uniform vec3 Kd; // Diffuse reflectivity

void main()
{
    // gouraud shading
    vec3 Position = vec3(ModelMatrix* vec4(VertexPosition, 1.0));
    vec3 Normal = mat3(transpose(inverse(ModelMatrix))) * VertexNormal;

    // ambient
    vec3 ambient = Ka * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir =  lightDirection;
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = Kd * diff * lightColor;
       
    Color = ambient +  diffuse ;//    
// gouraud shading end

    mat4 mvp=ProjectionMatrix * ViewMatrix * ModelMatrix;
    gl_Position = mvp * vec4(VertexPosition,1.0);
}
