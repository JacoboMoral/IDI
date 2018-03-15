#include <GL/glew.h>
#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QGLFormat &f, QWidget* parent) : QGLWidget(f, parent)
{
  setFocusPolicy(Qt::ClickFocus);  // per rebre events de teclat
  xClick = yClick = 0;
  angleY = 0/5.;
  angleX = 0/8.;
  DoingInteractive = NONE;
  alcadaModel = 7.5;
  movz = 0;
  magenta = false;
  franjes = 0;
}

void MyGLWidget::  colInit () {
    glm::vec3 colFocus(1,1,1);
    
    glUniform3fv(colorfocusLoc,1,&colFocus[0]);

    
}


void MyGLWidget::initializeGL ()
{
  // glew és necessari per cridar funcions de les darreres versions d'OpenGL
  glewExperimental = GL_TRUE;
  glewInit(); 
  glGetError();  // Reinicia la variable d'error d'OpenGL

  glClearColor(0.7, 0.7, 1.0, 1.0);  // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  loadShaders();
  createBuffers();
  calculs_noretall();
  initialize_Camera ();
  colInit();

}


void MyGLWidget::initialize_Camera ()
{
   viewTransform();
   //Calcul dels parametres inicials per a una camara perspectiva
   FOV_inicial = FOV = 2.0*asin(radiEsc/distancia);//(float)M_PI/2.f;
   ra = 1.f;
   //Calcul de parametres inicials per a una camara orthogonal
   r = t = radiEsc;
   l = b = -radiEsc;
   projectTransform();
}


void MyGLWidget::calculs_noretall() {
	//L'alçada del patricio max
	alcada_max = alcadaModel;
	//Prenem 5 i -5 pqe son els maxims i els minim del terra (i de l'escena per tant)
	maxim_escena = glm::vec3(10, alcada_max, 10);
	minim_escena = glm::vec3(-10, 0, -10); //alçada min = -1, que es on es situa el terra
	radiEsc = glm::distance(maxim_escena,minim_escena) / 2;
	distancia = radiEsc*2.f;
	//VRP = (maxim_escena+minim_escena); VRP /= 2;
	VRP[0] = (maxim_escena.x+minim_escena.x)/2.0;
	VRP[1] = (maxim_escena.y+minim_escena.y)/2.0;
	VRP[2] = (maxim_escena.z+minim_escena.z)/2.0;
}



void MyGLWidget::paintGL ()
{
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Activem el VAO per a pintar el terra 
  glBindVertexArray (VAO_Terra);

  modelTransformTerra ();

  // pintem
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  
  
  
  

  // Activem el VAO per a pintar el Patricio
  glBindVertexArray (VAO_Patr);

  modelTransformPatricio1 ();

  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
  
  modelTransformPatricio2 ();

  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
  
  modelTransformPatricio3 ();

  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
  
  
  
  
  
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h)
{
	ra = (float)w/(float)h; // ra(v) = raw
	//Recalculem el FOV per tal que no es retalli quan raw<1
	if (w < h) {
       FOV = 2 * atan(tan(FOV_inicial/2.f)/ra);
       b = -radiEsc / ra;
       t = radiEsc / ra;
   }
   if (w > h) {
       l = -radiEsc * ra;
       r = radiEsc * ra;
   }
  
	glViewport(0, 0, w, h);
	projectTransform();
}


void MyGLWidget::createBuffers ()
{
  // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
  patr.load("./models/Patricio.obj");

  // Calculem la capsa contenidora del model
  calculaCapsaModel ();
  
  // Creació del Vertex Array Object del Patricio
  glGenVertexArrays(1, &VAO_Patr);
  glBindVertexArray(VAO_Patr);

  // Creació dels buffers del model patr
  // Buffer de posicions
  glGenBuffers(1, &VBO_PatrPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatrPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3, patr.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Buffer de normals
  glGenBuffers(1, &VBO_PatrNorm);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatrNorm);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3, patr.VBO_normals(), GL_STATIC_DRAW);

  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // En lloc del color, ara passem tots els paràmetres dels materials
  // Buffer de component ambient
  glGenBuffers(1, &VBO_PatrMatamb);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatrMatamb);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3, patr.VBO_matamb(), GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glGenBuffers(1, &VBO_PatrMatdiff);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatrMatdiff);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3, patr.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glGenBuffers(1, &VBO_PatrMatspec);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatrMatspec);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3, patr.VBO_matspec(), GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glGenBuffers(1, &VBO_PatrMatshin);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatrMatshin);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3, patr.VBO_matshin(), GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  // Dades del terra
  // VBO amb la posició dels vèrtexs
  glm::vec3 posterra[4] = {
	glm::vec3( 10.0, 0.0, -10.0),
	glm::vec3( 10.0, 0.0,  10.0),
	glm::vec3(-10.0, 0.0, -10.0),
	glm::vec3(-10.0, 0.0,  10.0)
  }; 

  // VBO amb la normal de cada vèrtex
  glm::vec3 norm (0,1,0);
  glm::vec3 normterra[4] = {
	norm, norm, norm, norm
  };

  // Definim el material del terra
  glm::vec3 amb(0.3,0.3,0.0);
  glm::vec3 diff(1,1,0.0);
  glm::vec3 spec(1,1,1);
  float shin = 10000;

  // Fem que aquest material afecti a tots els vèrtexs per igual
  glm::vec3 matambterra[4] = {
	amb, amb, amb, amb
  };
  glm::vec3 matdiffterra[4] = {
	diff, diff, diff, diff
  };
  glm::vec3 matspecterra[4] = {
	spec, spec, spec, spec
  };
  float matshinterra[4] = {
	shin, shin, shin, shin
  };

// Creació del Vertex Array Object del terra
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);

  glGenBuffers(1, &VBO_TerraPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posterra), posterra, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_TerraNorm);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraNorm);
  glBufferData(GL_ARRAY_BUFFER, sizeof(normterra), normterra, GL_STATIC_DRAW);

  // Activem l'atribut normalLoc
  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // En lloc del color, ara passem tots els paràmetres dels materials
  // Buffer de component ambient
  glGenBuffers(1, &VBO_TerraMatamb);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraMatamb);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matambterra), matambterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glGenBuffers(1, &VBO_TerraMatdiff);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraMatdiff);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matdiffterra), matdiffterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glGenBuffers(1, &VBO_TerraMatspec);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraMatspec);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matspecterra), matspecterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glGenBuffers(1, &VBO_TerraMatshin);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraMatshin);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matshinterra), matshinterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  glBindVertexArray(0);
}

void MyGLWidget::loadShaders ()
{
  // Creem els shaders per al fragment i vertex shader
  QGLShader fs(QGLShader::Fragment, this);
  QGLShader vs(QGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem 
  fs.compileSourceFile("./shaders/fragshad.frag");
  vs.compileSourceFile("./shaders/vertshad.vert");

  // Creem el program
  program = new QGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);

  // Linkem el program
  program->link();

  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  // Obtenim identificador per a l'atribut “normal” del vertex shader
  normalLoc = glGetAttribLocation (program->programId(), "normal");
  // Obtenim identificador per a l'atribut “matamb” del vertex shader
  matambLoc = glGetAttribLocation (program->programId(), "matamb");
  // Obtenim identificador per a l'atribut “matdiff” del vertex shader
  matdiffLoc = glGetAttribLocation (program->programId(), "matdiff");
  // Obtenim identificador per a l'atribut “matspec” del vertex shader
  matspecLoc = glGetAttribLocation (program->programId(), "matspec");
  // Obtenim identificador per a l'atribut “matshin” del vertex shader
  matshinLoc = glGetAttribLocation (program->programId(), "matshin");
  franjesLoc = glGetUniformLocation (program->programId(), "franjes");
  // Demanem identificadors per als uniforms del vertex shader
  transLoc = glGetUniformLocation (program->programId(), "TG");
  projLoc = glGetUniformLocation (program->programId(), "proj");
  viewLoc = glGetUniformLocation (program->programId(), "view");
  colorfocusLoc = glGetUniformLocation (program->programId(), "colFocus");
}

void MyGLWidget::modelTransformPatricio1 ()
{
  glm::mat4 TG;  // Matriu de transformació
  TG = glm::translate(TG,glm::vec3(0,0,movz));
  TG = glm::translate(TG,glm::vec3(0,0,-8));
  TG = glm::scale(TG,glm::vec3(escalaPatr1,escalaPatr1,escalaPatr1));
  TG = glm::translate(TG, -centreBasePatr);  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPatricio2 ()
{
  glm::mat4 TG;  // Matriu de transformació
  TG = glm::translate(TG,glm::vec3(0,0,movz));
    TG = glm::translate(TG,glm::vec3(0,0,-8));
  TG = glm::translate(TG,glm::vec3(0,3,0));  
  TG = glm::scale(TG ,glm::vec3(escalaPatr2,escalaPatr2,escalaPatr2));
  TG = glm::rotate(TG, float(M_PI), glm::vec3(0, 1, 0));
  TG = glm::translate(TG, -centreBasePatr);  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPatricio3 ()
{
  glm::mat4 TG;  // Matriu de transformació
  TG = glm::translate(TG,glm::vec3(0,0,movz));
    TG = glm::translate(TG,glm::vec3(0,0,-8));
  TG = glm::translate(TG,glm::vec3(0,5.5,0));  
  TG = glm::scale(TG, glm::vec3(escalaPatr3,escalaPatr3,escalaPatr3));
  TG = glm::translate(TG, -centreBasePatr);  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformTerra ()
{
  glm::mat4 TG;  // Matriu de transformació
  TG = glm::mat4(1.f);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj;  // Matriu de projecció
  Proj = glm::perspective(FOV, ra, radiEsc, 3.0f*radiEsc);
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform ()
{
 glm::mat4 View;  // Matriu de posició i orientació
 View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -distancia)); //Allunyem camara
 View = glm::rotate(View, -angleX, glm::vec3(1, 0, 0));
 View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
 View = glm::translate(View, -VRP); // Portem la camara al VRP
 glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}


void MyGLWidget::calculaCapsaModel ()
{
  // Càlcul capsa contenidora i valors transformacions inicials
  minx = maxx = patr.vertices()[0];
  miny = maxy = patr.vertices()[1];
  minz = maxz = patr.vertices()[2];
  for (unsigned int i = 3; i < patr.vertices().size(); i+=3)
  {
    if (patr.vertices()[i+0] < minx)
      minx = patr.vertices()[i+0];
    if (patr.vertices()[i+0] > maxx)
      maxx = patr.vertices()[i+0];
    if (patr.vertices()[i+1] < miny)
      miny = patr.vertices()[i+1];
    if (patr.vertices()[i+1] > maxy)
      maxy = patr.vertices()[i+1];
    if (patr.vertices()[i+2] < minz)
      minz = patr.vertices()[i+2];
    if (patr.vertices()[i+2] > maxz)
      maxz = patr.vertices()[i+2];
  }
  escalaPatr1 = 3/(maxy-miny);
  escalaPatr2 = 2.5/(maxy-miny);
  escalaPatr3 = 2/(maxy-miny);
  centreBasePatr[0] = (minx+maxx)/2.0; centreBasePatr[1] = miny; centreBasePatr[2] = (minz+maxz)/2.0;
}

void MyGLWidget::keyPressEvent (QKeyEvent *e)
{
  switch (e->key())
  {
    case Qt::Key_Escape:
        exit(0);
    case Qt::Key_W: { // canvia òptica entre perspectiva i axonomètrica
        movz += 0.5;
        break;
    }
    case Qt::Key_S: { // canvia òptica entre perspectiva i axonomètrica
        movz -= 0.5;
        break;
    }
    case Qt::Key_L: { // canvia òptica entre perspectiva i axonomètrica
        if (magenta){
            magenta = false;
            glm::vec3 colFocus(1,0,1);
            glUniform3fv(colorfocusLoc,1,&colFocus[0]);
        }
        else{
            magenta = true;
            colInit();
        }
        break;
    }
    case Qt::Key_X: { // canvia òptica entre perspectiva i axonomètrica
        if (franjes == 1){
            franjes = 0;
            glUniform1f(franjesLoc,franjes);
        }
        else{
            franjes = 1;
            glUniform1f(franjesLoc,franjes);
        }
        break;
    }


    default: e->ignore(); break;
  }
  updateGL();
}

void MyGLWidget::mousePressEvent (QMouseEvent *e)
{
  xClick = e->x();
  yClick = e->y();

  if (e->button() & Qt::LeftButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = ROTATE;
  }
}

void MyGLWidget::mouseReleaseEvent( QMouseEvent *)
{
  DoingInteractive = NONE;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / 180.0;
    angleX += (yClick - e->y()) * M_PI / 180.0;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  updateGL ();
}


