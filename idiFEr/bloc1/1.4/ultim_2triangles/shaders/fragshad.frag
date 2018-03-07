#version 330 core

out vec4 FragColor;

void main() {
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
}

