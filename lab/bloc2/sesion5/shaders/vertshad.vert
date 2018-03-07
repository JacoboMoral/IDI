#version 330 core

in vec3 vertex;
in vec3 color;

uniform mat4 TG;

out vec3 fcolor;
uniform mat4 PR;

void main()  {
    fcolor = color;
    gl_Position = PR * TG * vec4 (vertex, 1.0);
}
