#version 330 core

out vec4 FragColor;
in vec3 ocolor;

void main ()
{
//     FragColor = vec4 (1.); //Exemple1
//     if ( gl_FragCoord .x < 357.) //exemple2
//         FragColor = vec4 (1. ,0. ,0. ,1.);

/* Ex 1 y 3
        if ( gl_FragCoord .x < 357.) {
            if( gl_FragCoord .y < 357. )
                FragColor = vec4 (1. ,1. ,0. ,1.); //Groc
            else
                FragColor = vec4 (1. ,0. ,0. ,0.); //Vermell
        }
        else {
            if( gl_FragCoord .y < 357. )
                FragColor = vec4 (0. ,1. ,0. ,0.); //Blau
            else
                FragColor = vec4 (0. ,0. ,1. ,0.); //Verd
        }
*/

/* Ex. 2

        int y = int(gl_FragCoord .y);
        if ( y/25%2 == 0) discard;
        
        if ( gl_FragCoord .x < 357.) {
            if( gl_FragCoord .y < 357. )
                FragColor = vec4 (1. ,1. ,0. ,1.); //Groc
            else
                FragColor = vec4 (1. ,0. ,0. ,0.); //Vermell
        }
        else {
            if( gl_FragCoord .y < 357. )
                FragColor = vec4 (0. ,1. ,0. ,0.); //Blau
            else
                FragColor = vec4 (0. ,0. ,1. ,0.); //Verd
        }
*/    

/* Ex. 4*/
        FragColor = vec4( ocolor, 0.);
    
}
