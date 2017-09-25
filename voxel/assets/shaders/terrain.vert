#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoords;
  
// Values that stay constant for the whole mesh.
uniform mat4 uModelViewProjection;

out vec2 texCoords;
  
void main(){
  // Output position of the vertex, in clip space : MVP * position
  gl_Position = uModelViewProjection * vec4(vertexPosition, 1);

  texCoords = vertexTexCoords;
}
