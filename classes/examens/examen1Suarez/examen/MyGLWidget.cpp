#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
  xClick = yClick = 0;
  angleY = angleX = 0.0;
  alcadaModel = 4;
  perspectiva = true;
  DoingInteractive = NONE;
  radiEsc = sqrt(5);
  rotar = 0.0;
  movz = movx = 0;

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
  glEnable (GL_DEPTH_TEST);
  carregaShaders ();
  createBuffers ();
  calculs_noretall();
  initialize_Camera ();
  initFocus();
}

void MyGLWidget::initFocus(){
        glm::vec3 posFocus = glm::vec3(movx,2,movz);// posicio del focus
        glUniform3fv(posFocusLoc,1,&posFocus[0]);
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
	maxim_escena = glm::vec3(2, 0.625, 2);
	minim_escena = glm::vec3(-2, -1, -2); //alçada min = -1, que es on es situa el terra
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
  glDrawArrays(GL_TRIANGLES, 0, 12);

  // Activem el VAO per a pintar el Patricio
  glBindVertexArray (VAO_Patr);
  

  
  
  modelTransformPatricio2 ();

  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
  
  
  
  
  glBindVertexArray (VAO_cow);
  
  
  modelTransformCow ();

  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
  
  
  
  
  glBindVertexArray(0);
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

  
  
  
  
  
  
  
  
  
  cow.load("./models/cow.obj");

  // Calculem la capsa contenidora del model
  calculaCapsaModelCow ();
  
  // Creació del Vertex Array Object del Patricio
  glGenVertexArrays(1, &VAO_cow);
  glBindVertexArray(VAO_cow);

  // Creació dels buffers del model patr
  // Buffer de posicions
  glGenBuffers(1, &VBO_cowPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_cowPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cow.faces().size()*3*3, cow.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Buffer de normals
  glGenBuffers(1, &VBO_cowNorm);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_cowNorm);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cow.faces().size()*3*3, cow.VBO_normals(), GL_STATIC_DRAW);

  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // En lloc del color, ara passem tots els paràmetres dels materials
  // Buffer de component ambient
  glGenBuffers(1, &VBO_cowMatamb);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_cowMatamb);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cow.faces().size()*3*3, cow.VBO_matamb(), GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glGenBuffers(1, &VBO_cowMatdiff);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_cowMatdiff);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cow.faces().size()*3*3, cow.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glGenBuffers(1, &VBO_cowMatspec);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_cowMatspec);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cow.faces().size()*3*3, cow.VBO_matspec(), GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glGenBuffers(1, &VBO_cowMatshin);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_cowMatshin);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cow.faces().size()*3, cow.VBO_matshin(), GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  // Dades del terra
  // VBO amb la posició dels vèrtexs
  
  int terraRadi = 20;
  
  glm::vec3 posterra[12] = {
	glm::vec3(-terraRadi, -1.0, terraRadi),
	glm::vec3(terraRadi, -1.0, terraRadi),
	glm::vec3(-terraRadi, -1.0, -terraRadi),
	glm::vec3(-terraRadi, -1.0, -terraRadi),
	glm::vec3(terraRadi, -1.0, terraRadi),
	glm::vec3(terraRadi, -1.0, -terraRadi),
	glm::vec3(-terraRadi, -1.0, -terraRadi),
	glm::vec3(terraRadi, -1.0, -terraRadi),
	glm::vec3(-terraRadi, 1.0, -terraRadi),
	glm::vec3(-terraRadi, 1.0, -terraRadi),
	glm::vec3(terraRadi, -1.0, -terraRadi),
	glm::vec3(terraRadi, 1.0, -terraRadi)
  }; 

  // VBO amb la normal de cada vèrtex
  glm::vec3 norm1 (0,1,0);
  glm::vec3 norm2 (0,0,1);
  glm::vec3 normterra[12] = {
	norm1, norm1, norm1, norm1, norm1, norm1, // la normal (0,1,0) per als primers dos triangles
	norm2, norm2, norm2, norm2, norm2, norm2  // la normal (0,0,1) per als dos últims triangles
  };

  // Definim el material del terra
  glm::vec3 amb(0.2,0,0.2);
  glm::vec3 diff(0.8,0,0.8);
  glm::vec3 spec(0,0,0);
  float shin = 100;

  // Fem que aquest material afecti a tots els vèrtexs per igual
  glm::vec3 matambterra[12] = {
	amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb
  };
  glm::vec3 matdiffterra[12] = {
	diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff
  };
  glm::vec3 matspecterra[12] = {
	spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec
  };
  float matshinterra[12] = {
	shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin
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

  // Demanem identificadors per als uniforms del vertex shader
  transLoc = glGetUniformLocation (program->programId(), "TG");
  projLoc = glGetUniformLocation (program->programId(), "proj");
  viewLoc = glGetUniformLocation (program->programId(), "view");
  posFocusLoc = glGetUniformLocation (program->programId(), "posFocus");
}

void MyGLWidget::modelTransformPatricio1 ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::scale(TG, glm::vec3(escala, escala, escala));
  TG = glm::translate(TG, -centrePatr);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPatricio2 ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::translate(TG, glm::vec3(movx,(-0.375),movz));
    TG = glm::rotate(TG, rotar, glm::vec3(0, 1, 0));
//   TG = glm::rotate(TG, float(M_PI), glm::vec3(0, 0, 1));
  TG = glm::scale(TG, glm::vec3(escala, escala, escala));
  TG = glm::translate(TG, -centrePatr);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformCow ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::translate(TG, glm::vec3(movx, -0.59, movz));
  TG = glm::rotate(TG, rotar, glm::vec3(0, 1, 0));
  TG = glm::rotate(TG, float(M_PI/-2.0), glm::vec3(1, 0, 0));
  TG = glm::scale(TG, glm::vec3(escalacow, escalacow, escalacow));
  TG = glm::translate(TG, -centrecow);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformTerra ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::projectTransform ()
{
 glm::mat4 Proj;  // Matriu de projecció
 if (perspectiva)
   Proj = glm::perspective(FOV, ra, radiEsc/2, 4.0f*radiEsc);
 else
   Proj = glm::ortho(l, r, b, t, radiEsc, 3.0f*radiEsc);
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
  escala = 0.25/(maxy-miny);
  centrePatr[0] = (minx+maxx)/2.0; centrePatr[1] = (miny+maxy)/2.0; centrePatr[2] = (minz+maxz)/2.0;
}


void MyGLWidget::calculaCapsaModelCow ()
{
  // Càlcul capsa contenidora i valors transformacions inicials
  minxcow = maxxcow = cow.vertices()[0];
  minycow = maxycow = cow.vertices()[1];
  minzcow = maxzcow = cow.vertices()[2];
  for (unsigned int i = 3; i < cow.vertices().size(); i+=3)
  {
    if (cow.vertices()[i+0] < minxcow)
      minxcow = cow.vertices()[i+0];
    if (cow.vertices()[i+0] > maxxcow)
      maxxcow = cow.vertices()[i+0];
    if (cow.vertices()[i+1] < minycow)
      minycow = cow.vertices()[i+1];
    if (cow.vertices()[i+1] > maxycow)
      maxycow = cow.vertices()[i+1];
    if (cow.vertices()[i+2] < minzcow)
      minzcow = cow.vertices()[i+2];
    if (cow.vertices()[i+2] > maxzcow)
      maxzcow = cow.vertices()[i+2];
  }
  escalacow = 0.5/(maxycow-minycow);
  centrecow[0] = (minxcow+maxxcow)/2.0;
  centrecow[1] = (minycow+maxycow)/2.0;
  centrecow[2] = (minzcow+maxzcow)/2.0;
}


void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_O: { // canvia òptica entre perspectiva i axonomètrica
      perspectiva = !perspectiva;
      projectTransform ();
      break;
    }
    
    case Qt::Key_R: { // canvia òptica entre perspectiva i axonomètrica
      rotar += float(M_PI/6.0);
      break;
    }
    
    case Qt::Key_Left: { // canvia òptica entre perspectiva i axonomètrica
      movx -= 1;
      initFocus();
      break;
    }
    
    case Qt::Key_Right: { // canvia òptica entre perspectiva i axonomètrica
      movx += 1;
      initFocus();
      break;
    }
    case Qt::Key_Up: { // canvia òptica entre perspectiva i axonomètrica
      movz -= 1;
      initFocus();
      break;
    }
    case Qt::Key_Down: { // canvia òptica entre perspectiva i axonomètrica
      movz += 1;
      initFocus();
      break;
    }
    
    default: event->ignore(); break;
  }
  update();
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
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / 180.0;
    angleX += (e->y() - yClick) * M_PI / 180.0;

    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}
void MyGLWidget::wheelEvent ( QWheelEvent * event )
{
    makeCurrent();
    float f = - event->delta()/120;
    if (((FOV + f/15) > 0) && ((FOV + f/15) < float(M_PI))) FOV += f/15;
    projectTransform();
    update ();
}
