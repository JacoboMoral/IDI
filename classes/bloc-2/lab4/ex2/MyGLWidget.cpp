#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
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
  //init optica i posicio de la camara
  ini_camera();
}

void MyGLWidget::paintGL () 
{

    //paint homer
  glClear (GL_COLOR_BUFFER_BIT);


  modelTransform ();
  glBindVertexArray (VAO);
  glDrawArrays(GL_TRIANGLES, 0, homer.faces().size()*3);


  //paint terra
  modelTransformTerra();
  glBindVertexArray(VAO_terra);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);



  glBindVertexArray (0);
}

void MyGLWidget::modelTransform () 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(scale));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformTerra ()
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(scale));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
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
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::modelHomer(){
    //load de les dades del homer
      homer.load("../../models/HomerProves.obj");


    // Creació del Vertex Array Object per pintar
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO_vertexos);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertexos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * homer.faces().size() * 3 * 3, homer.VBO_vertices(), GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glGenBuffers(1, &VBO_color);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * homer.faces().size() * 3 * 3, homer.VBO_matdiff(), GL_STATIC_DRAW);

    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);
}

void MyGLWidget::modelTerra(){
    //creacio dades del terra
    glm::vec3 posicio[4];
    posicio[0] = glm::vec3(-1.0, -1.0, -1.0);
    posicio[1] = glm::vec3(1.0, -1.0, -1.0);
    posicio[2] = glm::vec3(-1.0, -1.0, 1.0);
    posicio[3] = glm::vec3(1.0, -1.0, 1.0);

    glm::vec3 color[4];
    color[0] = glm::vec3(1,0,0);
    color[1] = glm::vec3(0,1,0);
    color[2] = glm::vec3(0,0,1);
    color[3] = glm::vec3(1,1,0);



    // Creació del Vertex Array Object per pintar
    glGenVertexArrays(1, &VAO_terra);
    glBindVertexArray(VAO_terra);

    glGenBuffers(1, &VBO_terravert);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_terravert);
    glBufferData(GL_ARRAY_BUFFER, sizeof(posicio), posicio, GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glGenBuffers(1, &VBO_terracol);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_terracol);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);
}

void MyGLWidget::createBuffers(){
  modelHomer();
  modelTerra();

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

  projLoc = glGetUniformLocation(program->programId(), "PM");

  viewLoc = glGetUniformLocation(program->programId(), "VM");
}

void MyGLWidget::projectTransform(){
    //Matriu de projeccio del model: perspective (fov (en radians), ra, znear, zfar)
    glm::mat4 projecta = glm::perspective(fov, ra, znear, zfar);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projecta[0][0]);
}

void MyGLWidget::viewTransform (){
      // Matriu de projecciÃ³ del model. lookAt(obs, vrp, vup)
      glm::mat4 view = glm::lookAt(obs, vrp, vup);
      glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
}

void MyGLWidget::ini_camera (){
    obs = glm::vec3(0,0,1);
    vrp = glm::vec3(0,0,0);
    vup = glm::vec3(0,1,0);
    viewTransform();

    fov = (float)M_PI/2.f;
    ra = 1.f;
    znear = 0.4f;
    zfar = 3.f;
    projectTransform();


}
