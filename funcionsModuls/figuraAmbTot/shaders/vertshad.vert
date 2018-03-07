#version 330 core
//ferran


in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;


out vec3 matambFS;
out vec3 matdiffFS;
out vec3 matspecFS;
out float matshinFS;

out vec4 vertexFS;
out vec3 normalFS;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;


void main()
{   //SCModel --TG--> SC Aplicacio --view--> SCObservador
    matambFS = matamb;
    matdiffFS = matdiff;
    matspecFS = matspec;
    matshinFS= matshin;
    
	vec4 vertexSCO = view * TG * vec4(vertex, 1.0); // En SCO
	vertexFS = vertexSCO;
	
	mat3 NormalMatrix = inverse(transpose (mat3 (view*TG)));
	vec3 normalSCO = normalize(NormalMatrix * normal);
	normalFS = normalSCO;
	
    gl_Position = proj * view * TG * vec4 (vertex, 1.0);
}
