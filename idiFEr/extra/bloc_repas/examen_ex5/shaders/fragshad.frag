#version 330 core

in vec3 fcolor;

uniform bool ratllat;

out vec4 FragColor;

void main()
{	
	vec3 fcolor2 = fcolor;
	int m = int(gl_FragCoord.y) % 20;
	
	if ( ratllat ) {
		if ( m > 0 && m < 10) {
			fcolor2 = vec3(0,0,0);
		}
		else {
			fcolor2 = vec3(1,1,1);			
		}
	}
	
	FragColor = vec4(fcolor2,1);	
			
}
