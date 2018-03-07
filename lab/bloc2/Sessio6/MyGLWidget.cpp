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
  rot = 0;
  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  carregaShaders();
  createBuffers();
  ini_camera();
}

void MyGLWidget::ini_camera()
{
  calc_capsacont();
  obs = glm::vec3(0,0,3*radi);//esta posat y=1 per poder veure el terra inicialment
  vrp = glm::vec3(0,0.5,0);//no es centre perque lhem mogut, es centre escena
  up = glm::vec3(0,1,1);
  fov = 2*asin(radi/(radi*3.f));
  f = fov;
  ra = 1.f;
  znear = 2*radi;
  zfar = 4.*radi;
  rotx = 0;
  roty = (float)M_PI/6.;
  axon = true;
  left = bottom = -radi;
  right = top = radi;
  projectTransform();
  viewTransform();
}

void MyGLWidget::paintGL () 
{
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Carreguem la transformació de model
  modelTransform ();

  // Activem el VAO per a pintar la caseta 
  glBindVertexArray (VAO_Pat);

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);
  
  modelTransform1 ();

  // Activem el VAO per a pintar la caseta 
  glBindVertexArray (VAO_Pat);

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);
  
  modelTransformTerra ();

  glBindVertexArray(VAO_Terra);
  
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindVertexArray (0);
}

void MyGLWidget::modelTransform () 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform, glm::vec3(-1,0,-1));
  transform = glm::rotate(transform, (float)M_PI, glm::vec3(0,1,0));
  transform = glm::scale(transform, glm::vec3(scale));
  transform = glm::translate(transform, -cbase);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransform1 () 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform, glm::vec3(1,0,1));
  transform = glm::scale(transform, glm::vec3(scale));
  transform = glm::translate(transform, -cbase);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformTerra () 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::projectTransform()//nomes es crida si canvien els parametres de la camara
{
  //glm::perspective(FOV en radians,ra window,znear,zfar)
  //glm::mat4 Proj = glm::ortho(left, right, bottom, top, znear, zfar);
  glm::mat4 Proj(1.0f);
  if (!axon) Proj = glm::perspective(f,ra,znear,zfar);
  else Proj = glm::ortho(left, right, bottom, top, znear, zfar);
  glUniformMatrix4fv(projLoc,1,GL_FALSE,&Proj[0][0]);
}

void MyGLWidget::viewTransform()
{
  //glm::lookAt(OBS,VRP,up);
  //glm::mat4 View = glm::lookAt(obs,vrp,up);
  glm::mat4 View (1.0f);
  //amb angles d'euler
  View = glm::translate(View,glm::vec3(0,0,-sqrt(.25+3.*3.*radi*radi)));//glm::vec3(0.,0.,-3*radi)
  View = glm::rotate(View, roty, glm::vec3(1.,0.,0.));
  View = glm::rotate(View, -rotx, glm::vec3(0.,1.,0.));
  View = glm::translate(View, -vrp);
  glUniformMatrix4fv(viewLoc,1,GL_FALSE,&View[0][0]);
}

void MyGLWidget::resizeGL (int w, int h) 
{
  ra = (float)w/(float)h;
  if(h > w){
    f = 2 * atan(tan(fov/2.f)/ra);
    bottom = -radi/ra;
    top = radi/ra;
  }
  if(w > h){
    left = -radi*ra;
    right = radi*ra;
  }
  glViewport(0, 0, w, h);
  projectTransform();
}

void MyGLWidget::calc_capsacont()
{
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = m.vertices()[0];
  miny = maxy = m.vertices()[1];
  minz = maxz = m.vertices()[2];
  for (unsigned int i = 3; i < m.vertices().size(); i+=3)
  {
    if (m.vertices()[i+0] < minx)
      minx = m.vertices()[i+0];
    if (m.vertices()[i+0] > maxx)
      maxx = m.vertices()[i+0];
    if (m.vertices()[i+1] < miny)
      miny = m.vertices()[i+1];
    if (m.vertices()[i+1] > maxy)
      maxy = m.vertices()[i+1];
    if (m.vertices()[i+2] < minz)
      minz = m.vertices()[i+2];
    if (m.vertices()[i+2] > maxz)
      maxz = m.vertices()[i+2];
  }
  centre[0]=(minx+maxx)/2.0;
  centre[1]=(miny+maxy)/2.0;
  centre[2]=(minz+maxz)/2.0;
  cbase[0] = centre[0];
  cbase[1] = miny;
  cbase[2] = centre[2];
  scale = 1./(maxy - miny);
  radi = sqrt((maxx-minx)*(maxx-minx)+(maxy-miny)*(maxy-miny)+(maxz-minz)*(maxz-minz))/2.;
  radi = sqrt(16.+1.+16.)/2.;
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
    case Qt::Key_R: { // escalar a més petit
      rot += (float)M_PI/4.f;
      break;
    }
    case Qt::Key_Z: {
      fov -= 0.1;
      f = 2 * atan(tan(fov/2.f)/ra);
      projectTransform();
      break;
    }
    case Qt::Key_X: {
      fov += 0.1;
      f = 2 * atan(tan(fov/2.f)/ra);
      projectTransform();
      break;
    }
    case Qt::Key_A: {//per canviar entre axon i persp
      axon = !axon;
      projectTransform();
      break;
    }
      //per fer zoom canviem el fov inicial, es a dir el fov
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::createBuffers () 
{
  // Dades de la caseta
  // Dos VBOs, un amb posició i l'altre amb color
  m.load("./models/Patricio.obj");

  glm::vec3 Vertices[4];
  Vertices[0] = glm::vec3(-2.,0.,-2.);
  Vertices[1] = glm::vec3(2.,0.,-2.);
  Vertices[2] = glm::vec3(-2.,0.,2.);
  Vertices[3] = glm::vec3(2.,0.,2.);

  // Creació del Vertex Array Object per pintar
  glGenVertexArrays(1, &VAO_Pat);
  glBindVertexArray(VAO_Pat);

  glGenBuffers(1, &VBO_PatPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_PatCol);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatCol);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_matdiff(), GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);
  
  glGenBuffers(1, &VBO_Terra);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

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
  
  projLoc = glGetUniformLocation(program->programId(), "proj");
  viewLoc = glGetUniformLocation(program->programId(), "view");
}

void MyGLWidget::mousePressEvent (QMouseEvent *e)
{
  makeCurrent();
  if (e->button() & Qt::LeftButton){
    clicx = (float)e->x();
    clicy = (float)e->y();
  }
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  rotx -= ((float)e->x()-clicx)*M_PI/180.0;
  clicx = (float)e->x();
  roty -= ((float)e->y()-clicy)*M_PI/180.0;
  clicy = (float)e->y();
  viewTransform();
  update();
}


