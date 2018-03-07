#version 330 core

in vec3 vertex;
uniform float escala;


void main()  {
    gl_Position = vec4 (escala * vertex, 1);
    
}
