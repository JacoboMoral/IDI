#version 330 core

out vec4 FragColor;

void main() {
    FragColor = vec4(1, 1, 1, 1);
    for(int i = 7; i < 700; i += 20){
        if(gl_FragCoord.y > i && gl_FragCoord.y < (i+10))
            discard;
    }
    if(gl_FragCoord.x < 357. && gl_FragCoord.y > 357.)
        FragColor = vec4(1,0,0,1);
    
    else if(gl_FragCoord.x > 357 && gl_FragCoord.y > 357)
        FragColor = vec4(0,0,1,1);
    
    else if(gl_FragCoord.x < 357 && gl_FragCoord.y < 357)
        FragColor = vec4(1,1,0,1);
    else FragColor = vec4(0,1,0,1);
}

