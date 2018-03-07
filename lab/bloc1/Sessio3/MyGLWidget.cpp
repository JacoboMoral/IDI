#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent)
{
  setFocusPolicy(Qt::ClickFocus);  // per rebre events de teclat
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
  scl = 0.5;
  scl2 = 0.5;
  tx = 0.;
  ty = 0.;
  rot = 0;
  clicx=0;
  clicy=0;
  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  createBuffers();
}

void MyGLWidget::modelTransform(){
  glm::mat4 TG(1.0);
  TG = glm::translate(TG, glm::vec3(tx,ty,0.0));
  TG = glm::rotate(TG, rot, glm::vec3(0.,0.,1.));
  TG = glm::scale(TG, glm::vec3(scl,scl2,1.));
  glUniformMatrix4fv(transLoc,1,GL_FALSE,&TG[0][0]);
}

void MyGLWidget::mousePressEvent(QMouseEvent *e){
  makeCurrent();
  if(e->button() & Qt::LeftButton){
    clicx = (float)e->x();
    clicy = (float)e->y();
  }
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e){
  makeCurrent();
  scl += ((float)e->x()-clicx)/600;
  clicx = (float)e->x();
  scl2 -= ((float)e->y()-clicy)/600;
  clicy = (float)e->y();
  update();
}

void MyGLWidget::keyPressEvent(QKeyEvent *e){
    makeCurrent();
    switch(e->key()){
        case Qt::Key_S:
            scl += 0.1;
        scl2 += 0.075;
            break;
        case Qt::Key_D:
            scl -= 0.1;
        scl2 -= 0.075;
            break;
	case Qt::Key_Left:
            rot += (float)M_PI/(float)4.;
        tx -= 0.1;
            break;
	case Qt::Key_Down:
            rot += (float)M_PI/(float)4.;
        ty -= 0.1;
            break;
	case Qt::Key_Right:
            rot += (float)M_PI/(float)4.;
        tx += 0.1;
            break;
	case Qt::Key_Up:
            rot += (float)M_PI/(float)4.;
        ty += 0.1;
            break;
        default: e->ignore();
    }
    update();
}

void MyGLWidget::paintGL ()
{
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

  // Activem l'Array a pintar 
  glBindVertexArray(VAO);
  modelTransform();
  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, 3);
  
  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h)
{
  glViewport (0, 0, w, h);
}

void MyGLWidget::createBuffers ()
{
  glm::vec3 Vertices[3];  // Tres vèrtexs amb X, Y i Z
  Vertices[0] = glm::vec3(-1.0, -1.0, 0.0);
  Vertices[1] = glm::vec3(1.0, -1.0, 0.0);
  Vertices[2] = glm::vec3(0.0, 1.0, 0.0);
  
  glm::vec3 Vertices2[3];  // Tres vèrtexs amb X, Y i Z
  Vertices2[0] = glm::vec3(1.0, 0.0, 0.0);
  Vertices2[1] = glm::vec3(0.0, 1.0, 0.0);
  Vertices2[2] = glm::vec3(0.0, 0.0, 1.0);
  
  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Creació del buffer amb les dades dels vèrtexs
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex	
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  
  glGenBuffers(1, &VBO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices2), Vertices2, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex	
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Desactivem el VAO
  glBindVertexArray(0);
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
  colorLoc = glGetAttribLocation (program->programId(), "color");
  //varLoc = glGetUniformLocation (program->programId(), "val");
  transLoc = glGetUniformLocation (program->programId(), "TG");
  //glUniform1f(varLoc, scl);

}
