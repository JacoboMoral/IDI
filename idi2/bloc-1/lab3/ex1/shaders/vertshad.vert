#version 330 core

in vec3 vertex;
in vec3 colorIn;
out vec4 colorOut;
uniform float scl;

void main()  {
    gl_Position = vec4 (scl * vertex, 1.0);
    colorOut = vec4(colorIn, 1.0);
}
