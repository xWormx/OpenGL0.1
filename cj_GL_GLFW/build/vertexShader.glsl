 #version 330 core  
 
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aCol;
layout (location = 2) in vec2 aTexCoord;

out vec4 colorFromVS;
out vec2 texCoord;

uniform vec3 uniPos;
uniform mat4 transform;

void main()
{
	vec3 position = aPos + uniPos;
	gl_Position = transform * vec4(aPos, 1.0);
	colorFromVS = aCol;
	texCoord = aTexCoord;
}

