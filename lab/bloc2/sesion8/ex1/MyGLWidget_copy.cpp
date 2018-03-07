#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent)
{
  setFocusPolicy(Qt::ClickFocus);  // per rebre events de teclat

  // constants
  escale = 1.0f;
  eixX   = glm::vec3(1.0, 0.0, 0.0);
  eixY   = glm::vec3(0.0, 1.0, 0.0);
  eixZ   = glm::vec3(0.0, 0.0, 1.0);

  xClick     = 0;
  yClick     = 0;
  deltaA     = M_PI / 180.0;
  interaccio = NOINTERACCIO;  
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

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  carregaShaders();
  createBuffers();

  inicialitzaCamera();
  
  viewTransform ();
  projectTransform ();
}

void MyGLWidget::paintGL () 
{
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Carreguem les transformacions de visió i projecció
  viewTransform ();
  projectTransform ();



  //pintem primer model
  modelTransform_HA();

  glBindVertexArray (VAO);
  glDrawArrays(GL_TRIANGLES, 0, 3 * model.faces().size());

  //pintem segon model
  modelTransform_HB();

  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 3 * model.faces().size());

  //pintar terra
  terraTransform();
  glBindVertexArray (VAO_terra);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray (0);
}


void MyGLWidget::resizeGL (int w, int h) 
{
  // Es crida quan canvien les dimensions de la vista
  // els parametres (w, h) corresponen a la nova vista!!
  // Es fixa el ratio al nou valor i
  // S'ajusta la finestra, si cal  

  float rViewport = float (w) / float (h);
  ra = rViewport;
  if (rViewport < 1.0) {
    fov = 2.0 * atan(tan(fovi/2.0) / rViewport);
  }
  
  // Es conserva la vista. Mesures en pixels.
  glViewport(0, 0, w, h);
}


void MyGLWidget::keyPressEvent (QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Z: { // zoom out
      if (fov < (float)M_PI) fov += deltaFov;
      break;
    }
    case Qt::Key_X: { // zoom in
      if (fov > 0.0) fov -= deltaFov;
      break;
    }
    default: event->ignore(); break;
  }
  update();
}


void MyGLWidget::mousePressEvent (QMouseEvent *e)
{
  makeCurrent();
  xClick = e->x();
  yClick = e->y();
  if (e->button() & Qt::LeftButton) {
    interaccio = ROTACIO;
  }else{
    interaccio = NOINTERACCIO;
  }
}


void MyGLWidget::mouseReleaseEvent(QMouseEvent *)
{
  makeCurrent();
  interaccio = NOINTERACCIO;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  if (interaccio == ROTACIO){
      makeCurrent();
      int dx = abs(e->x() - xClick);
      int dy = abs(e->y() - yClick);

      if (dx > dy){   // gir psi respecte eixY
        if (e->x() > xClick){
          psi  += abs(e->x() - xClick) * deltaA;
        }else if (e->x() < xClick){
          psi  -= abs(e->x() - xClick) * deltaA;
        }
      }else{  // gir theta respecte eixX
        if (e->y() > yClick){
          theta -= abs(e->y() - yClick) * deltaA;
        }else if (e->y() < yClick){
          theta += abs(e->y() - yClick) * deltaA;
        }
      }
  }
  update ();

  xClick = e->x();
  yClick = e->y();
}


void MyGLWidget::createBuffers () 
{
  // Càrrega del model
  model.load("/assig/idi/models/Patricio.obj");

  //model terra
  glm::vec3 terra_vertexs[4] = {
      glm::vec3(-1.0, -1.0, -1.0),
      glm::vec3(-1.0, -1.0, 1.0),
      glm::vec3(1.0, -1.0, -1.0),
      glm::vec3(1.0, -1.0, 1.0)
  };

  glm::vec3 terra_color[4] = {
      glm::vec3(48./255.,63./255.,159./255.),
      glm::vec3(48./255.,63./255.,159./255.),
      glm::vec3(48./255.,63./255.,159./255.),
      glm::vec3(48./255.,63./255.,159./255.),
};


  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO_terraV);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_terraV);
  glBufferData(GL_ARRAY_BUFFER, sizeof(terra_vertexs), terra_vertexs, GL_STATIC_DRAW);

  glGenBuffers(1, &VBO_terraC);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_terraC);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(terra_color), terra_color, GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Desactivem el VAO
  glBindVertexArray (0);


  // Creació del Vertex Array Object per pintar Homer
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO_vertexs);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_vertexs);
  glBufferData(GL_ARRAY_BUFFER,
	       sizeof(GLfloat) * model.faces().size() * 3 * 3,
	       model.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_color);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
  glBufferData(GL_ARRAY_BUFFER,
	       sizeof(GLfloat) * model.faces().size() * 3 * 3,
	       model.VBO_matdiff(), GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Desactivem el VAO
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

  // Obtenim identificadors pels atributs
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  colorLoc  = glGetAttribLocation (program->programId(), "color");

  // Identificadors pels uniform locations
  transLoc    = glGetUniformLocation(program->programId(), "TG");
  projectaLoc = glGetUniformLocation(program->programId(), "PR");
  viewLoc     = glGetUniformLocation(program->programId(), "VW");
}


void MyGLWidget::terraTransform(){
    glm::mat4 transform(1.0f);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}


void MyGLWidget::modelTransform_HA ()
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform,glm::vec3(1,0,1));
  transform = glm::scale(transform, glm::vec3(escala,escala,escala));
  transform = glm::translate(transform, -centreBasePatricio);  // T = Pfin - Pini

  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransform_HB ()
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform,glm::vec3(-1,0,-1));
  transform = glm::rotate(transform, (float)M_PI,eixY);
  transform = glm::scale(transform, glm::vec3(escala,escala,escala));
  transform = glm::translate(transform, -centreBasePatricio);  // T = Pfin - Pini

  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}


void MyGLWidget::viewTransform () 
{
  // Matriu de projecció del model
  // view = glm::lookAt(obs, vrp, vup);
  // Visió amb angles d'Euler
  glm::mat4 view(1.0f);
  view = glm::translate(view, -obs);      // trasllada la càmera a l'origen
  view = glm::rotate(view, -psi,  eixY);  // gira
  view = glm::rotate(view, theta, eixX);
  view = glm::translate(view, -vrp);      //trasllada el vrp a l'origen
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
}

void MyGLWidget::projectTransform () 
{
  // Matriu de projecció del model
  glm::mat4 projecta = glm::perspective(fov, ra, znear, zfar);
  glUniformMatrix4fv(projectaLoc, 1, GL_FALSE, &projecta[0][0]);
}


void MyGLWidget::inicialitzaCamera()
{
  radiEsferaContenidora();

  obs = glm::vec3(0.0, 0.0, 1.5 * radi);
  vrp = glm::vec3(0.0, 0.0, 0.0);
  vup = glm::vec3(0.0, 1.0, 0.0);

  psi    = 0.0;  // gir exiY
  theta  = 0.0;  // gir eixX
  phi    = 0.0;  // gir eix Z

  float d = 0;
  for (int i = 0; i < 3; i += 1){
    d = d + (obs[i] - vrp[i]) * (obs[i] - vrp[i]);
  }
  d     = sqrt(d);
  znear = (d - radi) / 2.0;
  zfar  = d + radi;
  fovi  = 2.0 * asin(radi / d); // (float)M_PI / 2.0f;
  fov   = fovi;
  ra    = 1.0;
  deltaFov = (float)M_PI / 50.0f;
}


void MyGLWidget::radiEsferaContenidora()
{
  float xmin, xmax, ymin, ymax, zmin, zmax;
  
  xmin = xmax = model.vertices()[0];
  ymin = ymax = model.vertices()[1];
  zmin = zmax = model.vertices()[2];
  for (unsigned int i = 3; i < model.vertices().size(); i += 3)
  {
    if (model.vertices()[i+0] < xmin)
      xmin = model.vertices()[i+0];
    if (model.vertices()[i+0] > xmax)
      xmax = model.vertices()[i+0];
    if (model.vertices()[i+1] < ymin)
      ymin = model.vertices()[i+1];
    if (model.vertices()[i+1] > ymax)
      ymax = model.vertices()[i+1];
    if (model.vertices()[i+2] < zmin)
      zmin = model.vertices()[i+2];
    if (model.vertices()[i+2] > zmax)
      zmax = model.vertices()[i+2];
  }

  centre[0] = (xmax + xmin) / 2.0;
  centre[1] = (ymax + ymin) / 2.0;
  centre[2] = (zmax + zmin) / 2.0;
  escala = 1.0 / (ymax - ymin);

  centreBasePatricio[0] = centre[0];
  centreBasePatricio[1] = ymin;
  centreBasePatricio[2] = centre[2];

  //inclusio del terra en el min/max
  if (xmax < 2.0) xmax = 2;
  if (xmin > -2.0) xmin = -2;
  if (zmax < 2.0) zmax = 2;
  if (zmin > -2.0) zmin = -2;


  
  float dx  = xmax - xmin;
  float dy  = ymax - ymin;
  float dz  = zmax - zmin;
  radi      = sqrt(dx * dx + dy * dy + dz * dz)/2.0;
}
