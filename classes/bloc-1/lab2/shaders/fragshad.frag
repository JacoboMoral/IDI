#version 330 core

out vec4 FragColor;
in vec4 colorOut;

void main() {
    /*for(int i = 7; i < 700; i = i +20){
    if( gl_FragCoord .y > i && gl_FragCoord .y < (i+10)) discard;
    }*/
    FragColor = colorOut;
    
    /*
    if( gl_FragCoord .x < 357. && gl_FragCoord .y < 357.)
        FragColor = vec4(1,1,0,0);
    if( gl_FragCoord .x < 357. && gl_FragCoord .y > 357.)
        FragColor = vec4(0,0,0,0);
    if( gl_FragCoord .x > 357. && gl_FragCoord .y > 357.)
        FragColor = vec4(1,1,0,0);
    */
}
