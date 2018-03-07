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

  // Activar el z-buffer(nomes cal fer-ho una vegada)
  glEnable(GL_DEPTH_TEST); 
  
  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  createBuffers();
  Camara3Persona(); //Inicializar la camara en tercera persona
  
  //inicializaciones de movimiento con raton
  xClick = yClick = 0;
  angulo_aumento = M_PI/360.0; //DELTA
  interaccio = NOINTERACCIO;
  
}

void MyGLWidget::paintGL () 
{
  // Esborrem el frame-buffer
  //glClear (GL_COLOR_BUFFER_BIT);
  
  //Per esborrar el buffer de profunditats a la vegada que el frame buffer  
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Carreguem la transformació de model
 
  modelTransform ();
  viewTransform ();
  //la view matrix como la vamos cambiando la tenemos que poner cada vez que pintamos
  // Activem el VAO per a pintar la caseta 
  glBindVertexArray (VAO_Homer);

  // pintem
  //glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);
  glDrawArrays(GL_TRIANGLES, 0, 3 * model.faces().size());

  glBindVertexArray (0);
  
  //TERRA
  /*modelTransform2 (); 
  glBindVertexArray (VAO_Terra);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray (0);*/
}

//matriu de transformacio para el homer
void MyGLWidget::modelTransform () 
{
  // Matriu de transformació de model
    //std::cout << scale << std::endl;
  glm::mat4 transform (1.0f);
  transform = glm::rotate(transform, alfa, glm::vec3(0,1,0));
  transform = glm::scale(transform, glm::vec3(scale));
  transform = glm::translate(transform, -centro); //trasladamos el patricio al origen de coordenadas
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

//matriu de transformacio para dejar quieto el suelo
void MyGLWidget::modelTransform2 () 
{
  // Matriu de transformació de model
    //std::cout << scale << std::endl;
  glm::mat4 transform2 (1.0f);
  transform2 = glm::scale(transform2, glm::vec3(scale));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform2[0][0]);
}

void MyGLWidget::projectTransform () {
    // glm::perspective (FOV en radians, ra window, znear, zfar)
    ////con los parametros inicializados en ini_camera()
    //glm::mat4 Proj = glm::perspective (FOV, raw, znear, zfar);
    glm::mat4 Proj = glm::perspective (FOV, raw, znear, zfar);
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform () {
    // glm::lookAt (OBS, VRP, UP)
    //con los parametros inicializados en ini_camera()
    //glm::mat4 View = glm::lookAt (OBS, VRP, up);
    //interaccion para el vector de up con Tecla
    //glm::mat4 View = glm::lookAt (OBS,VRP,up); //inicializados a mano
    
    //EULER
    glm::mat4 View(1.0f);
    View = glm::translate(View, glm::vec3(0,0,-distancia)); //-obs
    View = glm::rotate(View, angle2, glm::vec3(1,0,0));
    View = glm::rotate(View, -angle1, glm::vec3(0,1,0)); 
    View = glm::translate(View, glm::vec3(-VRP.x,-VRP.y,-VRP.z)); //es (0,0,0) ya que en este caso el centro de la "escena" esta en el origen de coordenadas
    
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}


//Funcion para Camara en 3ra persona
void MyGLWidget::Camara3Persona() {
    //calculamos el radio de la esfera contenidora del HOmer
    calculRadiEsfera();
    
    //Ponemos el observador al doble del radio 
    OBS = glm::vec3(0,0,2*radio);
    VRP = glm::vec3(0,0,0); //miramos en direccion al centro del homer/patricio
    up = glm::vec3(0,1,0); //de manera vertical
    
    distancia = 2*radio;
    FOVi = 2*asin(radio/distancia);
    FOV = FOVi;
    znear = distancia-radio; //znear justo delante de la escena
    zfar = distancia+radio;  //zfar justo detras de la escena
    raw = 1.0;
    alfa = 0; //angle de rotacion que usamos en la modelTransform()
    
    //llamamos a la matrices de visualizacion y proyeccion a partir de los parametros
    // de la camara en tercera persona iniciliazamos anteriormente
    viewTransform();
    projectTransform();
}

/* Para el homer
void MyGLWidget::calculRadiEsfera() {

    //Capsa contenidora
    float xmin, xmax;
    xmin = -1.0;
    xmax = 1.0;
    
    float ymin, ymax;
    ymin = -1.0;
    ymax = 1.0;
    
    float zmin, zmax;
    zmin = -1.0;
    zmax = 1.0;
    
    //distancias para poder calcular esfera
    float dx   = xmax - xmin; //1-(-1) = 2
    float dy   = ymax - ymin; //1-(-1) = 3
    float dz   = zmax - zmin; //1-(-1) = 2
    
    //diametro partido 2 = radio
    radio = sqrt(dx*dx + dy*dy + dz*dz)/2.0;
    
}*/


/*Calcul de la esfera del Patricio, y con ello tendremos el seu centre.
En este caso como en nuestra escena solo hay un Model, decimos que la 
esfera de la escena es la esfera de este modelo, pero si ubiesen mas modelos 
la esfera seria la esfera que contiende toda la escena !!! */
void MyGLWidget::calculRadiEsfera() {
    
    //Inicializamos los max-min al primer vertice, para poder hacer comparaciones    
    float xmin, xmax;
    xmin = xmax = model.vertices()[0];
    
    float ymin, ymax;
    ymin = ymax = model.vertices()[1];
    
    float zmin, zmax;
    zmin = zmax = model.vertices()[2];
    
    //Recorrremos el model.vertices() y vamos actualizando las coordenadas min y max
    for (unsigned int i = 3; i < model.vertices().size(); i += 3) {
        if(model.vertices()[i] < xmin) xmin = model.vertices()[i];
        if(model.vertices()[i] > xmax) xmax = model.vertices()[i];
        
        if(model.vertices()[i+1] < ymin) ymin = model.vertices()[i+1];
        if(model.vertices()[i+1] > ymax) ymax = model.vertices()[i+1];
        
        if(model.vertices()[i+2] < zmin) zmin = model.vertices()[i+2];
        if(model.vertices()[i+2] > zmax) zmax = model.vertices()[i+2];
    }

    float distx  = xmax - xmin;
    float disty  = ymax - ymin;
    float distz  = zmax - zmin;
    radio     = sqrt(distx * distx + disty * disty + distz * distz)/2.0;
    centro[0] = (xmax+xmin)/2;
    centro[1] = (ymax+ymin)/2;
    centro[2] = (zmax+zmin)/2;
    //para saber el centro
   // std::cout << centro[0] << " " << centro[1] << " " << centro[2] << " " << ymax << " " << ymin << std::endl;
}


//Me llegan el w i el h del VIEWPORT
// Es crida quan canvien les dimensions de la vista
// els parametres (w, h) corresponen a la nova vista!!    
// Es fixa el ratio al nou valor i
// S'ajusta la finestra (el fov), si cal 
void MyGLWidget::resizeGL (int w, int h) 
{
    //Calculamos la ra del vIewport    
    rav = float (w) / float (h);
    raw = rav; //la ra del window ha de ser igual a la del viwprt siempre
    //Pero, si la rav es menor que uno hemos de calcular un nuevo FOV,
    //siempre a partir del FOVi (inicial)
    if (rav < 1.0) {
        FOV = 2.0 * atan(tan(FOVi/2.0)/rav);
    }
    projectTransform(); //llamamos a la project transform para hacer los cambios del nuevo FOV
   
  glViewport(0, 0, w, h);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
    makeCurrent();
  switch (event->key()) {
    
      //Rotaciones del Objeto
   case Qt::Key_R: { 
      alfa += M_PI/4.0;  
      break;
    }
    case Qt::Key_T: { 
      alfa -= M_PI/4.0;  
      break;
    }
    case Qt::Key_S: { // escalar a més gran
      scale += 0.05;
      break;
    }
    case Qt::Key_D: { // escalar a més petit
      scale -= 0.05;
      break;
    }
    
    
    //euler, Se mueve el observador no el objeto
    case Qt::Key_Up: { // escalar a més petit
      angle2 += M_PI/20.0;
      break;
    }
    case Qt::Key_Down: { // escalar a més petit
      angle2 -= M_PI/20.0;
      break;
    }
    case Qt::Key_Left: { // escalar a més petit
      angle1 += M_PI/20.0;
      break;
    }
    case Qt::Key_Right: { // escalar a més petit
      angle1 -= M_PI/20.0;
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

//FUNCIONES PARA EL MOVIMIENTOM, CLICK DEL RATON
void MyGLWidget::mousePressEvent (QMouseEvent *e)
{
    makeCurrent();
    xClick = e->x(); //coord en x
    yClick = e->y(); //coord en y
    //interaccio con el click izquierdo
    if ((e->button() == Qt::LeftButton) & (e->button() != Qt::RightButton)) interaccio = MOVIMIENTO;
    else interaccio = NOINTERACCIO;
}

void MyGLWidget::mouseReleaseEvent(QMouseEvent *)
{
    makeCurrent();
    interaccio = NOINTERACCIO;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
    makeCurrent();
    int desplx = abs(e->x() - xClick);
    int desply = abs(e->y() - yClick);

    if (desplx > desply){   // giro respecto el eje Y "psi"
        //Recordar que angle1 esta negado en la viewTransform()
        if (e->x() > xClick){ // a la derecha
            angle1  -= abs(e->x() - xClick) * angulo_aumento;
        }
        else if (e->x() < xClick){ //a la izquierda
            angle1  += abs(e->x() - xClick) * angulo_aumento;
        }
    }
    else{  // giro respecto el eje X "theta"
        if (e->y() > yClick){ //hacia arriba
            angle2 += abs(e->y() - yClick) * angulo_aumento;
        }
        else if (e->y() < yClick){ //hacia abajo
            angle2 -= abs(e->y() - yClick) * angulo_aumento;
        }
    }
    update ();
    //Actualizamos las coordenadas del raton para el siguiente movimiento
    xClick = e->x();
    yClick = e->y();
}

void MyGLWidget::createBuffers () 
{
  //Creació del Vertex Array Object per pintar
  glGenVertexArrays(1, &VAO_Homer);
  glBindVertexArray(VAO_Homer);
  
  model.load("/dades/daniel.angel.perez/IDI/models/Patricio.obj");
  
  //vertexs del homer
  glGenBuffers(1, &VBO_vertexs);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_vertexs);
  glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat) * model.faces().size() * 3 * 3, model.VBO_vertices(), GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  
  //color del homor
  glGenBuffers(1, &VBO_color);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * model.faces().size() * 3 * 3, model.VBO_matdiff(), GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
  glBindVertexArray (0);
  
  //TERRA
  glm::vec3 posicio[4] = { 
    glm::vec3( 1.0, -1.0,  1.0),
    glm::vec3(-1.0, -1.0,  1.0),
    glm::vec3( 1.0, -1.0, -1.0),
    glm::vec3(-1.0, -1.0, -1.0)
  };
    
  glm::vec3 color[4] = {
	glm::vec3(1,0,0),
	glm::vec3(0,1,0),
	glm::vec3(0,0,1),
	glm::vec3(1,0,0),
  };
    
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);
  
  glGenBuffers(1, &VBO_posterra);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_posterra);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posicio), posicio, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  
  glGenBuffers(1, &VBO_TerraCol);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraCol);
  glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
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
  
  //unfirom para project matrix
  projLoc = glGetUniformLocation (program->programId(), "proj");
  //uniform para la view matrix
  viewLoc = glGetUniformLocation (program->programId(), "view");


}

