#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
// Notice that the "1" here equals the "1" in glVertexAttribPointer
layout(location = 1) in vec3 vertexColor;
layout (location = 2) in vec2 vertex_texCoord;

out vec2 texCoord;
out vec3 fragmentColor;

void main(){  
	gl_Position.xyz = vertexPosition_modelspace;
  	gl_Position.w = 1.0;

  	fragmentColor = vertexColor;
  	texCoord = vertex_texCoord;
}