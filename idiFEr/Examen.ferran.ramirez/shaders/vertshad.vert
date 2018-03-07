#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;


out vec3 matamb2;
out vec3 matdiff2;
out vec3 matspec2;
out float matshin2;

out vec4 vertexSCO2;
out vec3 normalSCO2;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;

void main()
{   //SCModel --TG--> SC Aplicacio --view--> SCObservador
    matamb2 = matamb;
    matdiff2 = matdiff;
    matspec2 = matspec;
    matshin2 = matshin;

	vec4 vertexSCO = view * TG * vec4(vertex, 1.0); // En SCO
	vertexSCO2 = vertexSCO;
	
	mat3 NormalMatrix = inverse(transpose (mat3 (view*TG)));
	vec3 normalSCO = normalize(NormalMatrix * normal);
	normalSCO2 = normalSCO;
	
    gl_Position = proj * view * TG * vec4 (vertex, 1.0);
}
