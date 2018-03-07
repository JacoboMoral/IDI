#version 330 core

in vec3 vertex;
in vec3 color;

uniform mat4 TG; //Transformacions model
uniform mat4 VM; //ViewMatrix - posicio/orientacio camara
uniform mat4 PM; //ProjectMatrix - optica de la camara

out vec3 fcolor;

void main()  {
    fcolor = color;
    gl_Position = PM * VM * TG * vec4 (vertex, 1.0);
}
