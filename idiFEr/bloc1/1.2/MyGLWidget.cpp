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
  
  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  createBuffers();
  createBuffers2();
}

void MyGLWidget::paintGL ()
{
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

  // Activem l'Array a pintar 
  glBindVertexArray(VAO);
 
  // Pintem l'escena
  /*ex2.1
   * glDrawArrays(GL_TRIANGLES, 0, 3);
  glDrawArrays(GL_TRIANGLES, 3, 3);*/
  
  /*ex2.2
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);*/
    
  /*ex3.1
   * glDrawArrays(GL_TRIANGLES, 0, 3);
  glDrawArrays(GL_TRIANGLES, 3, 3);
  glDrawArrays(GL_TRIANGLES, 6, 3);*/
  
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 7);
  
  glBindVertexArray(VAO2);
  
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  
  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h)
{
  glViewport (0, 0, w, h);
}

void MyGLWidget::createBuffers ()
{
  /*2.1
  glm::vec3 Vertices[6];  // Tres vèrtexs amb X, Y i Z
  Vertices[0] = glm::vec3(-0.7, -0.7, 0.0);
  Vertices[1] = glm::vec3(0.7, -0.7, 0.0);
  Vertices[2] = glm::vec3(0.7, 0.7, 0.0);
  Vertices[3] = glm::vec3(-0.7, -0.7, 0.0);
  Vertices[4] = glm::vec3(-0.7, 0.7, 0.0);
  Vertices[5] = glm::vec3(0.7, 0.7, 0.0);*/
  
  /*ex2.2
  glm::vec3 Vertices[4];
  Vertices[0] = glm::vec3(-0.7, -0.7, 0.0);
  Vertices[1] = glm::vec3(0.7, -0.7, 0.0);
  Vertices[2] = glm::vec3(-0.7, 0.7, 0.0);
  Vertices[3] = glm::vec3(0.7, 0.7, 0.0);*/
  
  /*ex3.1
  glm::vec3 Vertices[9]; 
  Vertices[0] = glm::vec3(-0.4, -0.4, 0.0);
  Vertices[1] = glm::vec3(0.4, -0.4, 0.0);
  Vertices[2] = glm::vec3(0.4, 0.4, 0.0);
  Vertices[3] = glm::vec3(-0.4, -0.4, 0.0);
  Vertices[4] = glm::vec3(-0.4, 0.4, 0.0);
  Vertices[5] = glm::vec3(0.4, 0.4, 0.0);
  Vertices[6] = glm::vec3(-0.6, 0.4, 0.0);
  Vertices[7] = glm::vec3(0.6, 0.4, 0.0);
  Vertices[8] = glm::vec3(0, 0.8, 0.0);*/
  
  //ex3.2 amb triangle strip
  glm::vec3 Vertices[7];  
  Vertices[0] = glm::vec3(-0.4, -0.4, 0.0);
  Vertices[1] = glm::vec3(0.4, -0.4, 0.0);
  Vertices[2] = glm::vec3(-0.4, 0.4, 0.0);
  Vertices[3] = glm::vec3(0.4, 0.4, 0.0);
  
  Vertices[4] = glm::vec3(0, 0.8, 0.0);
  Vertices[5] = glm::vec3(-0.6, 0.4, 0.0);
  Vertices[6] = glm::vec3(0.6, 0.4, 0.0);
  
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

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::createBuffers2 ()
{
  glm::vec3 Vertices[4];  
  Vertices[0] = glm::vec3(-1, -1, 0.0);
  Vertices[1] = glm::vec3(-1, -0.8, 0.0);
  Vertices[2] = glm::vec3(-0.8, -1, 0.0);
  Vertices[3] = glm::vec3(-0.8, -0.8, 0.0);
  
  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO2);
  glBindVertexArray(VAO2);

  // Creació del buffer amb les dades dels vèrtexs
  glGenBuffers(1, &VBO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex	
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

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
}
