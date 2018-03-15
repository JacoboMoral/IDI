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

  glClearColor (1, 1, 1, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  createBuffers();
  scl = 1.0;
  sclSpeed = 0.06;
  glUniform1f(varLoc, scl);

}

void MyGLWidget::paintGL ()
{
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

  // Activem l'Array a pintar
  glBindVertexArray(VAO);

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

  glm::vec3 Color[3];  // Tres vèrtexs amb X, Y i Z
    Color[0] = glm::vec3(1.0, 0.0, 0.0);
    Color[1] = glm::vec3(0.0, 1.0, 0.0);
    Color[2] = glm::vec3(0.0, 0.0, 1.0);

  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Creació del buffer amb les dades dels vèrtexs
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex
  pos = glGetAttribLocation(program->programId(), "vertex");

  glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(pos);

  // Creació del buffer amb les dades dels vèrtexs
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Color), Color, GL_STATIC_DRAW);

  // Activem l'atribut que farem servir per vèrtex
  color = glGetAttribLocation(program->programId(), "colorIn");
  glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(color);

  // Desactivem el VAO
  glBindVertexArray(pos);
  glBindVertexArray(color);
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

  // obtenim id per a l'atribut varloc del vertex shader
  varLoc = glGetUniformLocation(program->programId(), "scl");

}


void MyGLWidget::keyPressEvent (QKeyEvent *e) {
    makeCurrent ();
    switch ( e->key() ) {
        case Qt::Key_S :
            scl += sclSpeed;
            glUniform1f (varLoc, scl);
            break;
        case Qt::Key_D :
            scl -= sclSpeed;
            glUniform1f (varLoc, scl);
            break;

        case Qt::Key_A :
            sclSpeed += 0.05;
            break;

        case Qt::Key_F :
            if (sclSpeed > 0.05) sclSpeed -= 0.05;
            break;
        default: e->ignore (); // propagar al pare
    }
    update ();
}

