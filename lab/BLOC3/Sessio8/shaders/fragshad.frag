#version 330 core

out vec4 FragColor;

in vec3 NormSCO;
in vec4 VertSCO;

in vec3 matamb1;
in vec3 matdiff1;
in vec3 matspec1;
in float matshin1;

// Valors per als components que necessitem dels focus de llum
uniform vec3 colFocus;
uniform vec3 llumAmbient;
uniform vec3 posFocus;  // en SCA

vec3 Lambert (vec3 NormSCO, vec3 L) 
{
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats

    // Inicialitzem color a component ambient
    vec3 colRes = llumAmbient * matamb1;

    // Afegim component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colRes + colFocus * matdiff1 * dot (L, NormSCO);
    return (colRes);
}

vec3 Phong (vec3 NormSCO, vec3 L, vec4 vertSCO) 
{
    // Els vectors estan normalitzats

    // Inicialitzem color a Lambert
    vec3 colRes = Lambert (NormSCO, L);

    // Calculem R i V
    if (dot(NormSCO,L) < 0)
      return colRes;  // no hi ha component especular

    vec3 R = reflect(-L, NormSCO); // equival a: normalize (2.0*dot(NormSCO,L)*NormSCO - L);
    vec3 V = normalize(-vertSCO.xyz);

    if ((dot(R, V) < 0) || (matshin1 == 0))
      return colRes;  // no hi ha component especular
    
    // Afegim la component especular
    float shine = pow(max(0.0, dot(R, V)), matshin1);
    return (colRes + matspec1 * colFocus * shine); 
}

void main()
{
    //vec3 NormSCO1 = normalize(NormSCO);
    //vec4 posFocussco = view * vec4(posFocus,1.);
    //ara esta amb llum de camera
    vec3 L = normalize((posFocus - VertSCO.xyz));
    vec3 fcolor = Phong(normalize(NormSCO), L,VertSCO);
    FragColor = vec4(fcolor,1);	
}
