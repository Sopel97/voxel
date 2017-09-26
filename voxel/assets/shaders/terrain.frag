#version 330 core

in vec2 texCoords;

out vec4 color;

uniform sampler2D tex0;

void main(){
  vec4 color0 = texture(tex0, texCoords);
  color = color0;
  if(color.a < 0.01) discard;
}