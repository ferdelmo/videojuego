#version 330 core
in vec2 texCoord;
in vec3 fragmentColor;

uniform sampler2D sampTex;

out vec4 color;

void main(){
  color = texture(sampTex, texCoord);
}