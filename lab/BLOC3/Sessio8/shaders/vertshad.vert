#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

out vec3 matamb1;
out vec3 matdiff1;
out vec3 matspec1;
out float matshin1;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;
uniform mat3 NormalMatrix;

// Valors per als components que necessitem dels focus de llum
/*uniform vec3 colFocus;
uniform vec3 llumAmbient;
uniform vec3 posFocus;  // en SCA*/

out vec3 NormSCO;
out vec4 VertSCO;


void main()
{	
    matamb1 = matamb;
    matdiff1 = matdiff;
    matspec1 = matspec;
    matshin1 = matshin;
    //fcolor = matdiff;
    //mat3 NormalMatrix = inverse(transpose(mat3(view * TG)));
    NormSCO = normalize(NormalMatrix * normal);//vector normal sco normalitzat
    VertSCO = view * TG * vec4(vertex,1.);
    gl_Position = proj * VertSCO;
}