#version 330 core

out vec4 FragColor;
in vec3 fcolor;

void main() {
    /*if(int(gl_FragCoord.y)%8 < 4)
      discard;*/
    /*if(gl_FragCoord.x < 358. && gl_FragCoord.y < 358.)
      FragColor = vec4(1., 1., 0., 1);
    else if(gl_FragCoord.x >= 358. && gl_FragCoord.y < 358.)
      FragColor = vec4(0., 1., 0., 1);
    else if(gl_FragCoord.x >= 358. && gl_FragCoord.y >= 358.)
      FragColor = vec4(0., 0., 1., 1);
    else if(gl_FragCoord.x < 358. && gl_FragCoord.y >= 358.)
      FragColor = vec4(1., 0., 0., 1);*/
    FragColor = vec4(fcolor, 1.);
}

