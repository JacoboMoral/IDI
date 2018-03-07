#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent)
{
  setFocusPolicy(Qt::ClickFocus);  // per rebre events de teclat
  xClick = yClick = 0;
  angleY = angleX = 0.0;
  perspectiva = true;
  DoingInteractive = NONE;
  radiEsc = sqrt(5);
  alcadaModel = 3;
  segonfocus = false;
  eix_x = eix_z = 0;
  rotar = 0;


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

  calculs_noretall();
  initialize_Camera ();
}



void MyGLWidget::calculs_noretall() {
        //L'alçada del patricio max
        alcada_max = alcadaModel;
        //Prenem 5 i -5 pqe son els maxims i els minim del terra (i de l'escena per tant)
        maxim_escena = glm::vec3(20, alcada_max, 20);
        minim_escena = glm::vec3(-20, 0, -20); //alçada min = -1, que es on es situa el terra
        radiEsc = glm::distance(maxim_escena,minim_escena) / 2;
        distancia = radiEsc*2.f;
        //VRP = (maxim_escena+minim_escena); VRP /= 2;
        VRP[0] = (maxim_escena.x+minim_escena.x)/2.0;
        VRP[1] = (maxim_escena.y+minim_escena.y)/2.0;
        VRP[2] = (maxim_escena.z+minim_escena.z)/2.0;
}


void MyGLWidget::initialize_Camera ()
{
   viewTransform();
   //Calcul dels parametres inicials per a una camara perspectiva
   FOV_inicial = FOV = 2.0*asin(radiEsc/distancia);//(float)M_PI/2.f;
   emit emitZoom(FOV*180/M_PI);

   ra = 1.f;
   //Calcul de parametres inicials per a una camara orthogonal
   r = t = radiEsc;
   l = b = -radiEsc;
   projectTransform();
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

  // Activem el VAO per a pintar el Model
  glBindVertexArray (VAO_Model);

  modelTransformModelLego ();
  // Pintem Model
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);

  modelTransformModelLego2 ();
  // Pintem Model
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);

  modelTransformModelLego3();
  // Pintem Model
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);

  modelTransformModelLego4();
  // Pintem Model
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);

  modelTransformModelLego5 ();
  // Pintem Model
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);
  
  modelTransformModelLego6 ();
  // Pintem Model
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);

  modelTransformModelLego7 ();
  // Pintem Model
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);

  modelTransformModelLego8 ();
  // Pintem Model
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);

  modelTransformModelLego9 ();
  // Pintem Model
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);

  modelTransformModelLego10 ();
  // Pintem Model
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);



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




void MyGLWidget::modelTransformModelLego ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::translate(TG, glm::vec3(eix_x,0,eix_z));
  TG = glm::translate(TG, glm::vec3(3.5,0,0));
  TG = glm::rotate(TG, rotar, glm::vec3(1,1,0));
  TG = glm::scale(TG, glm::vec3(escalaModel,escalaModel,escalaModel));
  TG = glm::translate(TG, -centreBaseModel);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}
void MyGLWidget::modelTransformModelLego2 ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::translate(TG, glm::vec3(eix_x,0,eix_z));

  TG = glm::translate(TG, glm::vec3(-3.5,0,0));
  TG = glm::rotate(TG, rotar, glm::vec3(1,1,0));

  TG = glm::scale(TG, glm::vec3(escalaModel,escalaModel,escalaModel));
  TG = glm::translate(TG, -centreBaseModel);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}
void MyGLWidget::modelTransformModelLego3 ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::translate(TG, glm::vec3(eix_x,0,eix_z));

  TG = glm::translate(TG, glm::vec3(-10,0,0));
  TG = glm::rotate(TG, rotar, glm::vec3(1,1,0));

  TG = glm::scale(TG, glm::vec3(escalaModel,escalaModel,escalaModel));
  TG = glm::translate(TG, -centreBaseModel);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}
void MyGLWidget::modelTransformModelLego4 ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::translate(TG, glm::vec3(eix_x,0,eix_z));

  TG = glm::translate(TG, glm::vec3(10,0,0));
  TG = glm::rotate(TG, rotar, glm::vec3(1,1,0));

  TG = glm::scale(TG, glm::vec3(escalaModel,escalaModel,escalaModel));
  TG = glm::translate(TG, -centreBaseModel);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}
void MyGLWidget::modelTransformModelLego5 ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació

  TG = glm::translate(TG, glm::vec3(eix_x,0,eix_z));

  TG = glm::translate(TG, glm::vec3(-4,0,5));
  TG = glm::rotate(TG, rotar, glm::vec3(1,1,0));

  TG = glm::scale(TG, glm::vec3(escalaModel,escalaModel,escalaModel));
  TG = glm::translate(TG, -centreBaseModel);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}
void MyGLWidget::modelTransformModelLego6 ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::translate(TG, glm::vec3(eix_x,0,eix_z));

  TG = glm::translate(TG, glm::vec3(4,0,5));
  TG = glm::rotate(TG, rotar, glm::vec3(1,1,0));

  TG = glm::scale(TG, glm::vec3(escalaModel,escalaModel,escalaModel));
  TG = glm::translate(TG, -centreBaseModel);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}
void MyGLWidget::modelTransformModelLego7 ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::translate(TG, glm::vec3(eix_x,0,eix_z));

  TG = glm::translate(TG, glm::vec3(4,0,-5));
  TG = glm::rotate(TG, rotar, glm::vec3(1,1,0));

  TG = glm::scale(TG, glm::vec3(escalaModel,escalaModel,escalaModel));
  TG = glm::translate(TG, -centreBaseModel);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}
void MyGLWidget::modelTransformModelLego8 ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::translate(TG, glm::vec3(eix_x,0,eix_z));

  TG = glm::translate(TG, glm::vec3(-4,0,-5));
  TG = glm::rotate(TG, rotar, glm::vec3(1,1,0));

  TG = glm::scale(TG, glm::vec3(escalaModel,escalaModel,escalaModel));
  TG = glm::translate(TG, -centreBaseModel);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}
void MyGLWidget::modelTransformModelLego9 ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::translate(TG, glm::vec3(eix_x,0,eix_z));

  TG = glm::translate(TG, glm::vec3(0,0,10));
  TG = glm::rotate(TG, rotar, glm::vec3(1,1,0));

  TG = glm::scale(TG, glm::vec3(escalaModel,escalaModel,escalaModel));
  TG = glm::translate(TG, -centreBaseModel);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}
void MyGLWidget::modelTransformModelLego10 ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::translate(TG, glm::vec3(eix_x,0,eix_z));
  TG = glm::translate(TG, glm::vec3(0,0,-10));
  TG = glm::rotate(TG, rotar, glm::vec3(1,1,0));

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
 if (perspectiva)
   Proj = glm::perspective(FOV, ra, radiEsc, 3.0f*radiEsc);
 else
   Proj = glm::ortho(l, r, b, t, radiEsc, 3.0f*radiEsc);
 glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}
void MyGLWidget::canviarFocus ()
{
    float segonFocusEnviar = segonfocus ? 1.0 : 0.0;
    glUniform1f (boolFocusLoc, segonFocusEnviar);
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
        glm::vec3(-20.0, 0.0, -20.0),
        glm::vec3(-20.0, 0.0,  20.0),
        glm::vec3( 20.0, 0.0, -20.0),
        glm::vec3( 20.0, 0.0,  20.0)
  }; 

  // VBO amb la normal de cada vèrtex
  glm::vec3 normt (0,1,0);
  glm::vec3 normterra[4] = {
	normt, normt, normt, normt
  };

  // Definim el material del terra
  //glm::vec3 amb(0.1,0.1,0);
  //glm::vec3 diff(0.6,0.6,0.0);
  glm::vec3 spec(0.5,0.5,0.5);
  float shin = 100;

  // Fem que aquest material afecti a tots els vèrtexs per igual
  glm::vec3 matdiffterra[12] = {
        glm::vec3(1.0, 0.0, 0.0),
        glm::vec3(0.0, 1.0, 0.0),
        glm::vec3(0.0, 0.0, 1.0),
        glm::vec3(1.0, 1.0, 0.0),
        glm::vec3(0.0, 1.0, 0.0),
        glm::vec3(0.0, 0.0, 1.0),
        glm::vec3(1.0, 0.0, 0.0),
        glm::vec3(0.0, 1.0, 0.0),
        glm::vec3(0.0, 0.0, 1.0),
        glm::vec3(1.0, 0.0, 0.0),
        glm::vec3(0.0, 1.0, 0.0),
        glm::vec3(0.0, 0.0, 1.0)
    };

    glm::vec3 matambterra[12] = {
        glm::vec3(0.2, 0.0, 0.0),
        glm::vec3(0.8, 0.8, 0.0),
        glm::vec3(0.8, 0.8, 0.2),
        glm::vec3(0.8, 0.8, 0.0),
        glm::vec3(0.0, 0.2, 0.0),
        glm::vec3(0.0, 0.0, 0.2),
        glm::vec3(0.2, 0.0, 0.0),
        glm::vec3(0.0, 0.2, 0.0),
        glm::vec3(0.0, 0.0, 0.2),
        glm::vec3(0.2, 0.0, 0.0),
        glm::vec3(0.0, 0.2, 0.0),
        glm::vec3(0.0, 0.0, 0.2)
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
  boolFocusLoc = glGetUniformLocation (program->programId(), "boolfocus");

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
  escala = 7.0/(maxy-miny);
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
      case Qt::Key_L: { // canvia òptica entre perspectiva i axonomètrica
        segonfocus = !segonfocus;
        if(segonfocus) emit changeToSCA();
        else emit changeToSCO();

        canviarFocus();
        break;
      }
      case Qt::Key_Right: { // canvia òptica entre perspectiva i axonomètrica
        segonfocus = !segonfocus;
        eix_x += 0.5;
        break;
      }
      case Qt::Key_Left: { // canvia òptica entre perspectiva i axonomètrica
        segonfocus = !segonfocus;
        eix_x -= 0.5;
        break;
      }
      case Qt::Key_Up: { // canvia òptica entre perspectiva i axonomètrica
        segonfocus = !segonfocus;
        eix_z -= 0.5;
        break;
      }
      case Qt::Key_Down: { // canvia òptica entre perspectiva i axonomètrica
        segonfocus = !segonfocus;
        eix_z += 0.5;
        break;
      }
  case Qt::Key_R: { // canvia òptica entre perspectiva i axonomètrica
    rotar += M_PI/8.0;
    emit emitRotar(rotar*90/M_PI);

    break;
  }

  case Qt::Key_M: { // canvia òptica entre perspectiva i axonomètrica
    escalaModel += 0.2;
    break;
  }
  case Qt::Key_N: { // canvia òptica entre perspectiva i axonomètrica
    escalaModel -= 0.2;
    break;
  }
  case Qt::Key_Z: { // canvia òptica entre perspectiva i axonomètrica
    FOV += 0.1;
    projectTransform();
    emit emitZoom(FOV*180/M_PI);

    break;
  }
  case Qt::Key_X: { // canvia òptica entre perspectiva i axonomètrica
    FOV -= 0.1;
    projectTransform();
    emit emitZoom(FOV*180/M_PI);
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

void MyGLWidget::canviarASCA(bool SCA){
 makeCurrent();
 if(SCA)  segonfocus = true;
 canviarFocus();
 update();
}

void MyGLWidget::canviarASCO(bool SCO){
 makeCurrent();
 if(SCO) segonfocus = false;
 canviarFocus();
 update();
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

void MyGLWidget::zoomSlider(int value){
    makeCurrent();
    FOV = (M_PI/180.0)*value;
    projectTransform();
    update();

}

void MyGLWidget::rotarPatr(int rotarN){
    makeCurrent();
    rotar = (M_PI/90.0)*rotarN;
    update();
}




