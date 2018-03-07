#include "MyGLWidget.h"

#include <iostream>
using namespace std;

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent)
{
  setFocusPolicy(Qt::ClickFocus);  // per rebre events de teclat
  scale = 1.0f;
  
}

MyGLWidget::~MyGLWidget ()
{
	if (program != NULL)
		delete program;
}

void MyGLWidget::initializeGL ()
{
    //Incialitzacio dels atributs
	rotar = 0;
	escalat = 1;
	num_model = num_model2 = 0;
	red = green = blue = 0;
    calcul_Escena(); //Per donar valor a radi_escena
	
	w_paint = h_paint = 1;
	
    // Cal inicialitzar l'ús de les funcions d'OpenGL
    initializeOpenGLFunctions();
    
    //Esborrar buffer de profunditats alhora que el frame buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //Activar zbuffer
    glEnable(GL_DEPTH_TEST); // Per tal que gl no mostri "a través dels objectes"
    
    glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
    
    carregaShaders();
    createBuffers();
	
	costat_terra = 2;
    //Calculs inicials:
	calcul_Contenidora();
    //Inicialitzem optica i posicio de la camara
    initialize_Camera();
}

void MyGLWidget::initialize_Camera ()
{
    //radi_escena = 2; //Definit a la MyGLWidget per tal d'ajustar la mida del terra
    //radi_model = 1/2; //Radi del nostre model Patricio
	distancia = radi_escena*2.f;
    //Inicialitzacions d'orientacio/posicio (VM)
    OBS = glm::vec3(0,0,distancia);
    //VRP = glm::vec3(0,0.5,0); //centre de la escena a 0.5 -> patricios son d'alçada 1
    up = glm::vec3(0,1,0);
    calcul_Escena();
    
    viewTransform();

    //Inicialitzacions d'optica (PM) 0->perspective 1->othogonal
    optica = 0;
    
    FOV_inicial = FOV = 2.0*asin(radi_escena/distancia);//(float)M_PI/2.f;
    ra = 1.f;
    
    //Calcul de la distancia del centre del radi de la escena a la cantonada del terra per que no es retalli
    //sqrt(r^2 + r^2) = sqrt(2*r^2) = sqrt(2)*r
    Znear = distancia-costat_terra*sqrt(2);
    Zfar = distancia+costat_terra+sqrt(2);
    
    //Orthogonal/Axonometrica
    r = t = radi_escena;
    l = b = -radi_escena;
    
    projectTransform();
}

void MyGLWidget::paintGL () 
{
	
	//std::cout << "W_PAINT: " << w_paint << "   H_PAINT: " << h_paint << std::endl;
	float ample = width()*w_paint;
	float alt = height()*h_paint;
	// Els dos primers parametres de la crida glViewport son
	// per centrar la imatge un cop reduit el ViewPort
	glViewport( (width()-ample)/2, (height()-alt)/2, ample, alt);

	// Esborrem el frame-buffer
	glClear (GL_COLOR_BUFFER_BIT);

	// Carreguem la transformació de Model
	// Activem el VAO per a pintar el model i el pintem
	
    if (num_model == 0) {
		modelTransform ();
        glBindVertexArray (VAO_Model);
		glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3); 
	}
    else if(num_model == 1) {
		legoTransform ();
        glBindVertexArray (VAO_Lego);
		glDrawArrays(GL_TRIANGLES, 0, m2.faces().size()*3); 
	}
	
	if(num_model2 == 0) {
		modelTransform2 ();
		glBindVertexArray (VAO_Model);
		glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3); 
	}
    else if(num_model2 == 1) {
		homerTransform ();
        glBindVertexArray (VAO_Homer);
		glDrawArrays(GL_TRIANGLES, 0, m3.faces().size()*3); 
	}
	// Carreguem la transformació del Terra
	// Activem el VAO per a pintar el terra i el pintem
	modelTransform_Terra ();
	glBindVertexArray (VAO_Terra);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray (0); 
}

void MyGLWidget::modelTransform ()
{	// Matriu de transformació del Model
      
    glm::mat4 transform (1.0f);
    
	transform = glm::translate(transform, glm::vec3(1,0,1));  
    
	transform = glm::scale(transform, glm::vec3(scale));
	transform = glm::rotate(transform, rotar, glm::vec3(0,1,0));
    
	transform = glm::scale(transform, glm::vec3(fPatesc_y, fPatesc_y, fPatesc_y));
    
	transform = glm::translate(transform, -centre_base);  
	glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransform2 ()
{	// Matriu de transformació del Model
	glm::mat4 transform (1.0f);
    
	transform = glm::translate(transform, glm::vec3(-1,0,-1));  
	transform = glm::scale(transform, glm::vec3(fPatesc_y, fPatesc_y, fPatesc_y));
	transform = glm::rotate(transform, float(M_PI), glm::vec3(0,1,0));
    
	transform = glm::scale(transform, glm::vec3(scale));
	transform = glm::rotate(transform, rotar, glm::vec3(0,1,0));
    
	transform = glm::translate(transform, -centre_base);  
	glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::legoTransform ()
{	// Matriu de transformació del LegoMan
      
    glm::mat4 transform (1.0f);
    
	transform = glm::translate(transform, glm::vec3(1,0,1));  
    
	transform = glm::scale(transform, glm::vec3(scale));
	transform = glm::rotate(transform, rotar, glm::vec3(0,1,0));
    
	transform = glm::scale(transform, glm::vec3(fLegesc_y, fLegesc_y, fLegesc_y));
    
	transform = glm::translate(transform, -centre_base2);  
	glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::homerTransform ()
{	// Matriu de transformació del Homer
      
    glm::mat4 transform (1.0f);
    
	transform = glm::translate(transform, glm::vec3(-1,0,-1));  
    
	transform = glm::scale(transform, glm::vec3(scale));
	transform = glm::rotate(transform, rotar, glm::vec3(0,1,0));
    
	transform = glm::scale(transform, glm::vec3(fHomesc_y, fHomesc_y, fHomesc_y));
    
	transform = glm::rotate(transform, -90.f, glm::vec3(0,1,0));
	transform = glm::translate(transform, -centre_base3);  
	glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransform_Terra ()
{	// Matriu de transformació del terra
	glm::mat4 transform (1.0f);
	glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::viewTransform ()
{ 	// Matriu de transformació de la viewMatrix
    //glm::lookAt(OBS, VRP, up);
    
    glm::mat4 View (1.0f);
    //View = glm::lookAt(OBS, VRP, up);
	
	//Amb angles d'Euler
	View = glm::translate(View, glm::vec3(0,0,-distancia)); //Euler: Observador
	
    //Per rotar camara amb el ratoli
	View = glm::rotate(View, rotate_x, glm::vec3(1,0,0)); 
	View = glm::rotate(View, rotate_y, glm::vec3(0,1,0));
    
    //Per que es vegi el terra amb la ortogonal, petita rotacio
 	//View = glm::rotate(View, float(M_PI/16), glm::vec3(1,0,0));
    View = glm::translate(View, -VRP); //Euler: View Point Reference
    
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);   
}

void MyGLWidget::projectTransform ()
{ 	// Matriu de transformació de la projectMatrix
    //glm::perspective(FOV en radians,ra window,Znear,Zfar)
    //glm::ortho(l,r,b,t,Znear,Zfar)
    glm::mat4 Proj (1.0f);
    if (optica) Proj = glm::ortho(l,r,b,t,Znear,Zfar);
	else Proj = glm::perspective(FOV,ra,Znear,Zfar);
    
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::resizeGL (int w, int h) 
{    
	int w_prop = w * w_paint;
	int h_prop = h * h_paint;
	ra = (float)w_prop/(float)h_prop; // ra(v) = raw

	//std::cout << "input: " << w << "   " << h << std::endl;
	
	//Recalculem el FOV per tal que no es retalli quan raw<1
	if (w_prop < h_prop) {
        FOV = 2 * atan(tan(FOV_inicial/2.f)/ra);
        b = -radi_escena / ra;
        t = radi_escena / ra;
    }
    if (w_prop > h_prop) {
        l = -radi_escena * ra;
        r = radi_escena * ra;
    }
    
	projectTransform();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
	makeCurrent();
	switch (event->key()) {
		case Qt::Key_S: { // escalar a més gran
			scale += 0.05;
			break;
		}
		case Qt::Key_D: { // escalar a més petit
			scale -= 0.05;
			break;
		}
		case Qt::Key_R: { // rotar 45 graus
			rotar += (M_PI/4);
			break;
		}
		case Qt::Key_Z: { // Zoom-In
            //Condicio per posar limit maxim d'augment del model
            if(FOV > 0) FOV -= 0.1;
            projectTransform();
			break;
		}
		case Qt::Key_X: { // Zoom-Out
            //Condicio per posar limit maxim de reduccio del model
            if (FOV < 3) FOV += 0.1;
            projectTransform();
			break;
		}
        case Qt::Key_A: { // Per canviar l'optica
            optica = !optica;
            projectTransform();
            break;
        }
		default: event->ignore(); break;
	}
	update();
}

void MyGLWidget::mousePressEvent (QMouseEvent *e)
{
    makeCurrent();
    //If: No es prem cap modificador
    //&& !(e->modifiers() & (Qt::ShiftModifier | Qt::AltModifier | Qt::ControlModifier))
    
    //Controla que s’ha premut botó esquerre 
    if ( e->button() & Qt::LeftButton ){
        puntX_click = (float)e->x();
        puntY_click = (float)e->y();
    }
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
    makeCurrent();

    rotate_y += ((float)e->x() - puntX_click) * (M_PI/360);
    puntX_click = (float)e->x();

    rotate_x += ((float)e->y() - puntY_click) * (M_PI/360);
    puntY_click = (float)e->y();

    viewTransform();
    update();
    
	float gir_x = ((((float)rotate_y*180.) / (float)M_PI));
	float gir_y = ((((float)rotate_x*180.) / (float)M_PI));
	gir_x = int(gir_x)%(360);
	gir_y = int(gir_y)%(360);
	if (gir_y < 0) gir_y = 360 - gir_y;
	if (gir_x < 0) gir_x = 360 - gir_x;
	
	//Enviem signal
	emit angleY(gir_y);
	emit angleX(gir_x);
}

void MyGLWidget::bufferModel ()
{
	// Dades del nostre Model
    m.load("../../models/Patricio.obj");
	
	// Creació del Vertex Array Object per pintar
	glGenVertexArrays(1, &VAO_Model);
	glBindVertexArray(VAO_Model);

	glGenBuffers(1, &VBO_ModelPos);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ModelPos);
	glBufferData (GL_ARRAY_BUFFER,sizeof(GLfloat) * m.faces ().size () * 3 * 3, m.VBO_vertices (), GL_STATIC_DRAW);

	// Activem l'atribut vertexLoc
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertexLoc);

	glGenBuffers(1, &VBO_ModelCol);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ModelCol);
	glBufferData (GL_ARRAY_BUFFER,sizeof(GLfloat) * m.faces ().size () * 3 * 3, m.VBO_matdiff (), GL_STATIC_DRAW);

	// Activem l'atribut colorLoc
	glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colorLoc);
}

void MyGLWidget::bufferLego ()
{
	// Dades del nostre Model
    m2.load("../../models/legoman.obj");
	
	// Creació del Vertex Array Object per pintar
	glGenVertexArrays(1, &VAO_Lego);
	glBindVertexArray(VAO_Lego);

	glGenBuffers(1, &VBO_LegoPos);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_LegoPos);
	glBufferData (GL_ARRAY_BUFFER,sizeof(GLfloat) * m2.faces ().size () * 3 * 3, m2.VBO_vertices (), GL_STATIC_DRAW);

	// Activem l'atribut vertexLoc
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertexLoc);

	glGenBuffers(1, &VBO_LegoCol);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_LegoCol);
	glBufferData (GL_ARRAY_BUFFER,sizeof(GLfloat) * m2.faces ().size () * 3 * 3, m2.VBO_matdiff (), GL_STATIC_DRAW);

	// Activem l'atribut colorLoc
	glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colorLoc);
}

void MyGLWidget::bufferHomer ()
{
	// Dades del nostre model Homer
    m3.load("../../models/homer.obj");
	
	// Creació del Vertex Array Object per pintar
	glGenVertexArrays(1, &VAO_Homer);
	glBindVertexArray(VAO_Homer);

	glGenBuffers(1, &VBO_HomerPos);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_HomerPos);
	glBufferData (GL_ARRAY_BUFFER,sizeof(GLfloat) * m3.faces ().size () * 3 * 3, m3.VBO_vertices (), GL_STATIC_DRAW);

	// Activem l'atribut vertexLoc
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertexLoc);

	glGenBuffers(1, &VBO_HomerCol);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_HomerCol);
	glBufferData (GL_ARRAY_BUFFER,sizeof(GLfloat) * m3.faces ().size () * 3 * 3, m3.VBO_matdiff (), GL_STATIC_DRAW);

	// Activem l'atribut colorLoc
	glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colorLoc);
}

void MyGLWidget::bufferTerra ()
{
	glm::vec3 Vertices[4];
	costat_terra = 2;
	Vertices[0] = glm::vec3(-costat_terra, 0, -costat_terra);
	Vertices[1] = glm::vec3(costat_terra, 0, -costat_terra);
	Vertices[2] = glm::vec3(-costat_terra, 0, costat_terra);
	Vertices[3] = glm::vec3(costat_terra, 0, costat_terra);
	
	glm::vec3 Color[4];
	Color[0] = glm::vec3(red,green,blue);
	Color[1] = glm::vec3(red,green,blue);
	Color[2] = glm::vec3(red,green,blue);
	Color[3] = glm::vec3(red,green,blue);
		
	glGenVertexArrays(1, &VAO_Terra);
	glBindVertexArray(VAO_Terra);

	glGenBuffers(1, &VBO_Terra);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	// Activem l'atribut vertexLoc
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertexLoc);

	glGenBuffers(1, &VBO_TerraCol);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraCol);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Color), Color, GL_STATIC_DRAW);
	
	// Activem l'atribut colorLoc
	glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colorLoc);
}

void MyGLWidget::createBuffers ()
{
	//Buffer dels models
	bufferModel();
	bufferHomer();
	bufferLego();
	bufferModel();
    
	//Buffer del terra
	bufferTerra();
	
	glBindVertexArray (0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/fragshad.frag");
  vs.compileSourceFile("shaders/vertshad.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  // Obtenim identificador per a l'atribut “color” del vertex shader
  colorLoc = glGetAttribLocation (program->programId(), "color");
  // Uniform locations
  transLoc = glGetUniformLocation(program->programId(), "TG");
  viewLoc = glGetUniformLocation(program->programId(), "VM");
  projLoc = glGetUniformLocation(program->programId(), "PM");
}

void MyGLWidget::calcul_Contenidora() 
{	//Calculem els punts maxim, minim i central de la capsa contenidora
	
    std::vector<Vertex> v = m.vertices();
    
	puntmax.x = puntmin.x = v[0];
	puntmax.y = puntmin.y = v[1];
	puntmax.z = puntmin.z = v[2];
	
    for (unsigned int i = 3; i < v.size(); i+=3) {
		if( v[i] > puntmax.x )
			puntmax.x = v[i];
		if( v[i+1] > puntmax.y )
			puntmax.y = v[i+1];
		if( v[i+2] > puntmax.z )
			puntmax.z = v[i+2];
		
		if( v[i] < puntmin.x )
			puntmin.x = v[i];
		if( v[i+1] < puntmin.y )
			puntmin.y = v[i+1];
		if( v[i+2] < puntmin.z )
			puntmin.z = v[i+2];
    }
    float x = (puntmax.x + puntmin.x)/2;
    float y = (puntmax.y + puntmin.y)/2;
    float z = (puntmax.z + puntmin.z)/2;
    centre_esfera = glm::vec3(x,y,z);
    centre_base = glm::vec3(x,puntmin.y,z);
    
    /*x = (puntmax.x - centre_esfera.x);
    y = (puntmax.y - centre_esfera.y);
    z = (puntmax.z - centre_esfera.z);*/
    radi_model = glm::distance(puntmax,centre_esfera);
    
    fPatesc_y = escalat/(puntmax.y-puntmin.y);  
    
    //LEGOMAN
    v = m2.vertices();
	puntmax.x = puntmin.x = v[0];
	puntmax.y = puntmin.y = v[1];
	puntmax.z = puntmin.z = v[2];
	
    for (unsigned int i = 3; i < v.size(); i+=3) {
		if( v[i] > puntmax.x )
			puntmax.x = v[i];
		if( v[i+1] > puntmax.y )
			puntmax.y = v[i+1];
		if( v[i+2] > puntmax.z )
			puntmax.z = v[i+2];
		
		if( v[i] < puntmin.x )
			puntmin.x = v[i];
		if( v[i+1] < puntmin.y )
			puntmin.y = v[i+1];
		if( v[i+2] < puntmin.z )
			puntmin.z = v[i+2];
    }
    x = (puntmax.x + puntmin.x)/2;
    y = (puntmax.y + puntmin.y)/2;
    z = (puntmax.z + puntmin.z)/2;
    
    centre_base2 = glm::vec3(x,puntmin.y,z);
    
    fLegesc_y = escalat/(puntmax.y-puntmin.y);  
    
    //HOMER
    v = m3.vertices();
	puntmax.x = puntmin.x = v[0];
	puntmax.y = puntmin.y = v[1];
	puntmax.z = puntmin.z = v[2];
	
    for (unsigned int i = 3; i < v.size(); i+=3) {
		if( v[i] > puntmax.x )
			puntmax.x = v[i];
		if( v[i+1] > puntmax.y )
			puntmax.y = v[i+1];
		if( v[i+2] > puntmax.z )
			puntmax.z = v[i+2];
		
		if( v[i] < puntmin.x )
			puntmin.x = v[i];
		if( v[i+1] < puntmin.y )
			puntmin.y = v[i+1];
		if( v[i+2] < puntmin.z )
			puntmin.z = v[i+2];
    }
    x = (puntmax.x + puntmin.x)/2;
    y = (puntmax.y + puntmin.y)/2;
    z = (puntmax.z + puntmin.z)/2;
    
    centre_base3 = glm::vec3(x,puntmin.y,z);
    
    fHomesc_y = escalat/(puntmax.y-puntmin.y);  
}	

void MyGLWidget::calcul_Escena() 
{
	VRP = glm::vec3(0,escalat/2.0,0); //centre de la escena a 0.5 -> models son d'alçada 1
	
	glm::vec3 max, min;
	
	max.x = max.z = 2;
	min.x = min.z = -2;
	max.y = escalat;
	min.y = 0;
	
    radi_escena = glm::distance(max,VRP);
}

//Funcions que reben un SIGNAL
void MyGLWidget::changeZoom(int angle) 
{
    makeCurrent();
    //Perspective Zoom
    FOV = ((angle*M_PI)/180);
    //Orthogonal Zoom
    l = b = (-angle*0.18);
    r = t = (angle*0.18);
    projectTransform();
    update();
}

void MyGLWidget::chargePatricio() { num_model = 0; repintar(); }
void MyGLWidget::chargePatricio2() { num_model2 = 0; repintar(); }
void MyGLWidget::chargeLego() { num_model = 1; repintar(); }
void MyGLWidget::chargeHomer() { num_model2 = 1; repintar(); }
void MyGLWidget::repintar() 
{	
	makeCurrent();
	update();
}

void MyGLWidget::setPerspective()
{
    makeCurrent();
	optica = false;
    projectTransform();
    update();
}

void MyGLWidget::setOrthogonal()
{
    makeCurrent();
	optica = true;
    projectTransform();
    update();
}

void MyGLWidget::setEscala(int esc)
{
    makeCurrent();
    escalat = esc;	
	calcul_Contenidora();
	initialize_Camera(); //Per que al escalar la escena estigui centrada al nou VRPs
    update();
}

void MyGLWidget::eulerX(int graus)
{
    makeCurrent();
    rotate_y = ((float)graus * (float)M_PI/180.);
    viewTransform();
    update();
}

void MyGLWidget::eulerY(int graus)
{
    makeCurrent();
    rotate_x = ((float)graus * (float)M_PI/180.);
    viewTransform();
    update();
}

void MyGLWidget::viewportW(int prop)
{
    makeCurrent();
    w_paint = (prop/100.0);
    resizeGL(width(), height());
	update();
}

void MyGLWidget::viewportH(int prop)
{
    makeCurrent();
    h_paint = (prop/100.0);
    resizeGL(width(), height());
	update();
}

//Rebem signal de MyQLabel
void MyGLWidget::changeTerraColor(int rednew, int greennew, int bluenew)
{
    makeCurrent();
    red = (float)rednew/255;
    green = (float)greennew/255;
    blue = (float)bluenew/255;
    bufferTerra();
    update();
}

