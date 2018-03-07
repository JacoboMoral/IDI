#version 330 core

in vec3 vertex;
in vec3 color;
out vec3 ocolor;

void main()  {
    gl_Position = vec4 (vertex/2, 1.0); // canviar de 1 a 2 per l'escala
    ocolor = color;
}
