#include "MyGLWidget.h"

#include <iostream>

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
    // Cal inicialitzar l'ús de les funcions d'OpenGL
    initializeOpenGLFunctions();
    
    //Esborrar buffer de profunditats alhora que el frame buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //Activar zbuffer
    glEnable(GL_DEPTH_TEST); // Per tal que gl no mostri "a través dels objectes"
    
    glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)

    carregaShaders();
    createBuffers();
	
	rotar = 0;
    //Inicialitzem optica i posicio de la camara
    initialize_Camera();
}

void MyGLWidget::initialize_Camera ()
{
    //Inicialitzacions d'orientacio/posicio (VM)
    OBS = glm::vec3(0,0,1);
    VRP = glm::vec3(0,0,0);
    up = glm::vec3(0,1,0);
    viewTransform();

    //Inicialitzacions d'optica (PM)
    FOV = (float)M_PI/2.f;
    ra = 1.f;
    Znear = 0.4f;
    Zfar = 3.f;
    projectTransform();
}

void MyGLWidget::paintGL () 
{
	// Esborrem el frame-buffer
	glClear (GL_COLOR_BUFFER_BIT);

	// Carreguem la transformació de Model
	// Activem el VAO per a pintar el model i el pintem
	modelTransform ();
	glBindVertexArray (VAO_Model);
	glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3); 
	
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
	transform = glm::scale(transform, glm::vec3(scale));
	transform = glm::rotate(transform, rotar, glm::vec3(0,1,0));
	glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransform_Terra ()
{	// Matriu de transformació del terra
	glm::mat4 transform (1.0f);
	transform = glm::scale(transform, glm::vec3(scale));
	glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::viewTransform ()
{ 	// Matriu de transformació de la viewMatrix
    //glm::lookAt(OBS, VRP, up);
    
    glm::mat4 View = glm::lookAt(OBS, VRP, up);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::projectTransform ()
{ 	// Matriu de transformació de la projectMatrix
    //glm::perspective(FOV en radians,ra window,Znear,Zfar)
    //glm::ortho(l,r,b,t,Znear,Zfar)
    
    //glm::mat4 Proj = glm::ortho(-1.f,1.f,-1.f,1.f,Znear,Zfar);
	glm::mat4 Proj = glm::perspective(FOV,ra,Znear,Zfar);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::resizeGL (int w, int h) 
{
	glViewport(0, 0, w, h);
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
		default: event->ignore(); break;
	}
	update();
}

void MyGLWidget::bufferModel ()
{
	// Dades del nostre Model
	m.load("../../models/HomerProves.obj");
	
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

void MyGLWidget::bufferTerra ()
{
	glm::vec3 Vertices[4];
	Vertices[0] = glm::vec3(-1.0, -1.0, -1.0);
	Vertices[1] = glm::vec3(1.0, -1.0, -1.0);
	Vertices[2] = glm::vec3(-1.0, -1.0, 1.0);
	Vertices[3] = glm::vec3(1.0, -1.0, 1.0);	
		
	glm::vec3 Color[4];
	Color[0] = glm::vec3(1,0,0);
	Color[1] = glm::vec3(0,1,0);
	Color[2] = glm::vec3(0,0,1);
	Color[3] = glm::vec3(1,1,0);
		
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
	//Buffer del model - HOMER
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

