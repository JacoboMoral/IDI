#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent)
{
  setFocusPolicy(Qt::ClickFocus);  // per rebre events de teclat
  xClick = yClick = 0;
  angleY = angleX = 0;//M_PI/4.0; //Angle de rotació de la escena per defecte a 0
  perspectiva = true;
  DoingInteractive = NONE;
  radiEsc = sqrt(5);
  
  alcadaModel = 5; //Volem que tots els nostres Patricios tinguin una alçada de 5
  
  green = 1; // Component G del focus al máxim
  rotar = 0; // Valor inicial rotació de la rotllana de Patricios
  
  //Inicialitzacions per a la segona camera
  secondCamera = false; // desactivada per defecte
  OBS2 = glm::vec3(0, 2.5, 0); // On situarem la camara
  VRP2 = glm::vec3(0, 2.5, -2.5); // Per tal de mirar cap a les Z-
  FOV2 = float(M_PI/2.0);
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
  createBuffersModel();
  createBuffersTerra();
  
  ini_color();
  calculs_noretall();
  initialize_Camera();
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

void MyGLWidget::ini_color() {
	//Definicio del color de focus, que enviem com a uniform als shaders
	colFocus = glm::vec3(1, green, 1);
	glUniform3fv(colorfocusLoc,1,&colFocus[0]);
}

void MyGLWidget::paintGL () 
{
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Activem el VAO per a pintar el terra 
  glBindVertexArray (VAO_Terra);

  modelTransformIdent ();
  // pintem terra
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  modelTransformModel ();
  // Activem el VAO per a pintar el Model
  glBindVertexArray (VAO_Model);
  // Pintem Model
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);

  modelTransformModel2 ();
  // Activem el VAO per a pintar el Model
  glBindVertexArray (VAO_Model);
  // Pintem Model
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);

  modelTransformModel3 ();
  // Activem el VAO per a pintar el Model
  glBindVertexArray (VAO_Model);
  // Pintem Model
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);

  modelTransformModel4 ();
  // Activem el VAO per a pintar el Model
  glBindVertexArray (VAO_Model);
  // Pintem Model
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);
  
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h) 
{
	ra = (float)w/(float)h; // ra(v) = raw

	if (w < h) {
		//Recalculem el FOV per tal que no es retalli quan raw<1 (optica perspectiva)
        FOV = 2 * atan(tan(FOV_inicial/2.f)/ra);
        //Recalculem parametres per a l'optica orthogonal
        b = -radiEsc / ra;
        t = radiEsc / ra;
    }
    if (w > h) {
        //Recalculem parametres per a l'optica orthogonal
        l = -radiEsc * ra;
        r = radiEsc * ra;
    }
    
	glViewport(0, 0, w, h);
	projectTransform();
}

void MyGLWidget::calculs_noretall() {
	//L'alçada maxima del nostre model patricio en aquesta escena
	//alcadaModel definida a l'inici amb l'alçada que es demanava pels Patricios
	alcada_max = alcadaModel;
	//Prenem 10 i -10 pqe son els maxims i els minim del terra (i de l'escena per tant)
	maxim_escena = glm::vec3(10, alcada_max, 10); 
	minim_escena = glm::vec3(-10, 0, -10);

	radiEsc = glm::distance(maxim_escena,minim_escena) / 2;
	distancia = radiEsc*2.f;

	//VRP = (maxim_escena+minim_escena); VRP /= 2;
	VRP[0] = (maxim_escena.x+minim_escena.x)/2.0;
	VRP[1] = (maxim_escena.y+minim_escena.y)/2.0;
	VRP[2] = (maxim_escena.z+minim_escena.z)/2.0;
}

void MyGLWidget::modelTransformModel ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::rotate(TG, rotar, glm::vec3(0,1,0)); // rotacio de la rotllana
  TG = glm::translate(TG, glm::vec3(-2.5, 0, 0));
  TG = glm::rotate(TG, float(M_PI/2), glm::vec3(0,1,0)); //Mira a les X+
  TG = glm::scale(TG, glm::vec3(escalaModel,escalaModel,escalaModel));
  TG = glm::translate(TG, -centreBaseModel);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformModel2 ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::rotate(TG, rotar, glm::vec3(0,1,0)); // rotacio de la rotllana
  TG = glm::translate(TG, glm::vec3(0, 0, 2.5));
  TG = glm::rotate(TG, float(M_PI), glm::vec3(0,1,0)); //Mira a les Z-
  TG = glm::scale(TG, glm::vec3(escalaModel,escalaModel,escalaModel));
  TG = glm::translate(TG, -centreBaseModel);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformModel3 ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::rotate(TG, rotar, glm::vec3(0,1,0)); // rotacio de la rotllana
  TG = glm::translate(TG, glm::vec3(2.5, 0, 0));
  TG = glm::rotate(TG, float(M_PI/2), glm::vec3(0,-1,0)); //Mira a les X-
  TG = glm::scale(TG, glm::vec3(escalaModel,escalaModel,escalaModel));
  TG = glm::translate(TG, -centreBaseModel);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformModel4 ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::rotate(TG, rotar, glm::vec3(0,1,0)); // rotacio de la rotllana
  TG = glm::translate(TG, glm::vec3(0, 0, -2.5));
  //TG = glm::rotate(TG, float(0), glm::vec3(0,1,0)); Ja mira a les Z+
  TG = glm::scale(TG, glm::vec3(escalaModel,escalaModel,escalaModel));
  TG = glm::translate(TG, -centreBaseModel);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformIdent ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj;  // Matriu de projecció
  if( secondCamera ) {
    Proj = glm::perspective(FOV2, ra, 0.1f, radiEsc);
}
  else {
	if (perspectiva)
	  Proj = glm::perspective(FOV, ra, radiEsc, 3.0f*radiEsc);
	else
	  Proj = glm::ortho(l, r, b, t, radiEsc, 3.0f*radiEsc);
  }
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform ()
{
  glm::mat4 View;  // Matriu de posició i orientació
  if( secondCamera ) {
	View = glm::lookAt(OBS2, VRP2, glm::vec3(0, 1, 0)); // Camara definida per a la "segonacamara"
  }
  else {
    View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -distancia)); //Allunyem camara
	View = glm::rotate(View, -angleX, glm::vec3(1, 0, 0));
	View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0)); 
	View = glm::translate(View, -VRP); // Portem la camara al VRP
  }
  

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::createBuffersModel ()
{
  // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
  m.load("./models/Patricio.obj");

  // Calculem la capsa contenidora del model
  calculaCapsaModel (m, escalaModel, centreBaseModel);
  
  // Creació del Vertex Array Object del Model
  glGenVertexArrays(1, &VAO_Model);
  glBindVertexArray(VAO_Model);

  // Creació dels buffers del model patr
  // Buffer de posicions
  glGenBuffers(1, &VBO_ModelPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_ModelPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Buffer de normals
  glGenBuffers(1, &VBO_ModelNorm);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_ModelNorm);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_normals(), GL_STATIC_DRAW);

  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // En lloc del color, ara passem tots els paràmetres dels materials
  // Buffer de component ambient
  glGenBuffers(1, &VBO_ModelMatamb);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_ModelMatamb);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_matamb(), GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glGenBuffers(1, &VBO_ModelMatdiff);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_ModelMatdiff);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glGenBuffers(1, &VBO_ModelMatspec);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_ModelMatspec);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_matspec(), GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glGenBuffers(1, &VBO_ModelMatshin);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_ModelMatshin);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3, m.VBO_matshin(), GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  glBindVertexArray(0);
}

void MyGLWidget::createBuffersTerra ()
{
  // Dades del terra
  // VBO amb la posició dels vèrtexs
  glm::vec3 posterra[4] = {
        glm::vec3(-10.0, 0.0, -10.0),
        glm::vec3(-10.0, 0.0,  10.0),
        glm::vec3( 10.0, 0.0, -10.0),
        glm::vec3( 10.0, 0.0,  10.0)
  }; 

  // VBO amb la normal de cada vèrtex
  glm::vec3 normt (0,1,0);
  glm::vec3 normterra[4] = {
	normt, normt, normt, normt
  };

  // Definim el material del terra
  glm::vec3 amb(0,0,0);
  glm::vec3 diff(0.3,0.3,0);
  glm::vec3 spec(0.6,0.6,1); // Amb la component especular definim el terra d'un material brillant
  float shin = 100;

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
  
  colorfocusLoc = glGetUniformLocation (program->programId(), "colFocus");
}

void MyGLWidget::calculaCapsaModel (Model &p, float &escala, glm::vec3 &centreBase)
{
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = p.vertices()[0];
  miny = maxy = p.vertices()[1];
  minz = maxz = p.vertices()[2];
  for (unsigned int i = 3; i < p.vertices().size(); i+=3)
  {
    if (p.vertices()[i+0] < minx)
      minx = p.vertices()[i+0];
    if (p.vertices()[i+0] > maxx)
      maxx = p.vertices()[i+0];
    if (p.vertices()[i+1] < miny)
      miny = p.vertices()[i+1];
    if (p.vertices()[i+1] > maxy)
      maxy = p.vertices()[i+1];
    if (p.vertices()[i+2] < minz)
      minz = p.vertices()[i+2];
    if (p.vertices()[i+2] > maxz)
      maxz = p.vertices()[i+2];
  }
  // Posem el 5, perque es la mida a la que volem escalar la Y dels nostres models
  escala = alcadaModel/(maxy-miny);
  centreBase[0] = (minx+maxx)/2.0; centreBase[1] = miny; centreBase[2] = (minz+maxz)/2.0;
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
    /*PROVES
     * case Qt::Key_G: { // redueix color verd del focus
      green -= 0.1;
      ini_color ();
      break;
    }
    case Qt::Key_H: { // augmenta color verd del focus
      green = 0.1;
      ini_color ();
      break;
    }
    case Qt::Key_R: { // rotar rotllana dels Patricios
      rotar += (M_PI/4);
      break;
    }*/
    case Qt::Key_C: { // alternar les 2 cameres
	  secondCamera = !secondCamera;
	  viewTransform();
	  projectTransform();
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

//Slots
//(EX4) Rebem un angle de rotacio que indica la posicio del spatricios respecte l'eix Y
void MyGLWidget::doRotllana(int rotation) {
    makeCurrent();
	rotar = (float)rotation * (float)M_PI/180.;
	update();
}

//(EX6) Rebem un numero que indica el valor del component G del nostre focus de llum
void MyGLWidget::changeGreen(int color) {
    makeCurrent();
	green = (float)color/255;
	ini_color();
	update();
}




