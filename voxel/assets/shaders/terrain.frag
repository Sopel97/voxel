#version 330 core

in vec2 texCoords;

out vec4 color;

uniform sampler2D tex0;

void main(){
  vec4 color = texture(tex0, texCoords);
  if(color.a < 0.01) discard;
}