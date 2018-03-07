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

  // Activar el z-buffer(nomes cal fer-ho una vegada)
  glEnable(GL_DEPTH_TEST); 
  
  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  createBuffers();
  Camara3Persona(); //Inicializar la camara en tercera persona
 
}

void MyGLWidget::paintGL () 
{
  // Esborrem el frame-buffer
  //glClear (GL_COLOR_BUFFER_BIT);
  
  //Per esborrar el buffer de profunditats a la vegada que el frame buffer  
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Carreguem la transformació de model
 
  modelTransform ();
  viewTransform ();
  //la view matrix como la vamos cambiando la tenemos que poner cada vez que pintamos
  // Activem el VAO per a pintar la caseta 
  glBindVertexArray (VAO_Homer);

  // pintem
  //glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);
  glDrawArrays(GL_TRIANGLES, 0, 3 * model.faces().size());

  glBindVertexArray (0);
  
  modelTransform2 (); 
  glBindVertexArray (VAO_Terra);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray (0);
}

//matriu de transformacio para el homer
void MyGLWidget::modelTransform () 
{
  // Matriu de transformació de model
    //std::cout << scale << std::endl;
  glm::mat4 transform (1.0f);
  transform = glm::rotate(transform, alfa, glm::vec3(0,1,0));
  transform = glm::scale(transform, glm::vec3(scale));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

//matriu de transformacio para dejar quieto el suelo
void MyGLWidget::modelTransform2 () 
{
  // Matriu de transformació de model
    //std::cout << scale << std::endl;
  glm::mat4 transform2 (1.0f);
  transform2 = glm::scale(transform2, glm::vec3(scale));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform2[0][0]);
}

void MyGLWidget::projectTransform () {
    // glm::perspective (FOV en radians, ra window, znear, zfar)
    ////con los parametros inicializados en ini_camera()
    //glm::mat4 Proj = glm::perspective (FOV, raw, znear, zfar);
    glm::mat4 Proj = glm::perspective (FOV, raw, znear, zfar);
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform () {
    // glm::lookAt (OBS, VRP, UP)
    //con los parametros inicializados en ini_camera()
    //glm::mat4 View = glm::lookAt (OBS, VRP, up);
    //interaccion para el vector de up con Tecla
    glm::mat4 View = glm::lookAt (OBS,VRP,up);
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

//Funcion para Camara en 3ra persona
void MyGLWidget::Camara3Persona() {
    //calculamos el radio de la esfera contenidora del HOmer
    calculRadiEsfera();
    
    //Ponemos el observador al doble del radio 
    OBS = glm::vec3(0,0,2*radio);
    VRP = glm::vec3(0,0,0); //miramos en direccion al centro del homer
    up = glm::vec3(0,1,0); //de manera vertical
    
    float d = 2*radio;
    FOVi = 2*asin(radio/d);
    FOV = FOVi;
    znear = d-radio; //znear justo delante de la escena
    zfar = d+radio;  //zfar justo detras de la escena
    raw = 1.0;
    alfa = 0; //angle de rotacion que usamos en la modelTransform()
    
    //llamamos a la matrices de visualizacion y projeccion
    viewTransform();
    projectTransform();
}


void MyGLWidget::calculRadiEsfera() {

    //Capsa contenidora
    float xmin, xmax;
    xmin = -1.0;
    xmax = 1.0;
    
    float ymin, ymax;
    ymin = -1.0;
    ymax = 1.0;
    
    float zmin, zmax;
    zmin = -1.0;
    zmax = 1.0;
    
    //distancias para poder calcular esfera
    float dx   = xmax - xmin; //1-(-1) = 2
    float dy   = ymax - ymin; //1-(-1) = 3
    float dz   = zmax - zmin; //1-(-1) = 2
    
    //diametro partido 2 = radio
    radio = sqrt(dx*dx + dy*dy + dz*dz)/2.0;
    
}


//Me llegan el w i el h del VIEWPORT
// Es crida quan canvien les dimensions de la vista
// els parametres (w, h) corresponen a la nova vista!!    
// Es fixa el ratio al nou valor i
// S'ajusta la finestra (el fov), si cal 
void MyGLWidget::resizeGL (int w, int h) 
{
    //Calculamos la ra del vewport    
    rav = float (w) / float (h);
    raw = rav; //la ra del window ha de ser igual a la del viwprt siempre
    //Pero, si la rav es menor que uno hemos de calcular un nuevo FOV,
    //siempre a partir del FOVi (inicial)
    if (rav < 1.0) {
        FOV = 2.0 * atan(tan(FOVi/2.0)/rav);
    }
    projectTransform(); //llamamos a la project transform para hacer los cambios
   
  glViewport(0, 0, w, h);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
    makeCurrent();
  switch (event->key()) {

   case Qt::Key_R: { //interaccion para el vector de UP
      alfa += M_PI/4.0;  
      break;
    }
    case Qt::Key_T: { //interaccion para el vector de UP
      alfa -= M_PI/4.0;  
      break;
    }
    case Qt::Key_S: { // escalar a més gran
      scale += 0.05;
      break;
    }
    case Qt::Key_D: { // escalar a més petit
      scale -= 0.05;
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::createBuffers () 
{
  // Dades de la caseta
  // Dos VBOs, un amb posició i l'altre amb color
  /*glm::vec3 posicio[5] = {
	glm::vec3(-0.5, -1.0, -0.5),
	glm::vec3( 0.5, -1.0, -0.5),
	glm::vec3(-0.5,  0.0, -0.5),
	glm::vec3( 0.5,  0.0, -0.5),
	glm::vec3( 0.0,  0.6, -0.5)
  }; 
  glm::vec3 color[5] = {
	glm::vec3(1,0,0),
	glm::vec3(0,1,0),
	glm::vec3(0,0,1),
	glm::vec3(1,0,0),
	glm::vec3(0,1,0)
  };*/

  /*glGenBuffers(1, &VBO_CasaPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_CasaPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posicio), posicio, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_CasaCol);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_CasaCol);
  glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);*/

  //Creació del Vertex Array Object per pintar
  glGenVertexArrays(1, &VAO_Homer);
  glBindVertexArray(VAO_Homer);
  
  model.load("/dades/daniel.angel.perez/IDI/models/HomerProves.obj");
  
  //vertexs del homer
  glGenBuffers(1, &VBO_vertexs);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_vertexs);
  glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat) * model.faces().size() * 3 * 3, model.VBO_vertices(), GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  
  //color del homor
  glGenBuffers(1, &VBO_color);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * model.faces().size() * 3 * 3, model.VBO_matdiff(), GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
  glBindVertexArray (0);
  
  
   glm::vec3 posicio[4] = {
         glm::vec3( 1.0, -1.0,  1.0),
         glm::vec3(-1.0, -1.0,  1.0),
         glm::vec3( 1.0, -1.0, -1.0),
         glm::vec3(-1.0, -1.0, -1.0)
    };
    
  glm::vec3 color[4] = {
	glm::vec3(1,0,0),
	glm::vec3(0,1,0),
	glm::vec3(0,0,1),
	glm::vec3(1,0,0),
  };
    
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);
  
  glGenBuffers(1, &VBO_posterra);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_posterra);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posicio), posicio, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  
  glGenBuffers(1, &VBO_TerraCol);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraCol);
  glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
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
  
  //unfirom para project matrix
  projLoc = glGetUniformLocation (program->programId(), "proj");
  //uniform para la view matrix
  viewLoc = glGetUniformLocation (program->programId(), "view");


}

