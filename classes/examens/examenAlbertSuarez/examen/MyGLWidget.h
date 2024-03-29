#define GLM_FORCE_RADIANS
#include <QGLWidget>
#include <QGLShader>
#include <QGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "model.h"

class MyGLWidget : public QGLWidget 
{
  Q_OBJECT

  public:
    MyGLWidget (QGLFormat &f, QWidget *parent=0);
  
  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ();

    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ();
 
    // resizeGL - Es cridat quan canvi la mida del widget
    virtual void resizeGL (int width, int height);  

    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event); 
    // mouse{Press/Release/Move}Event - Són cridades quan es realitza l'event 
    // corresponent de ratolí
    virtual void mousePressEvent (QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent *event);
    virtual void mouseMoveEvent (QMouseEvent *event);

  private:
    void createBuffers ();
    void loadShaders ();
    void projectTransform ();
    void viewTransform ();
    void modelTransformTerra ();
    void modelTransformPatricio1 ();
    void modelTransformPatricio2 ();
    void modelTransformPatricio3 ();
    void colInit();
    void calculaCapsaModel ();
    
    void calculs_noretall();
    void initialize_Camera();
    
    
    bool magenta;
    float FOV_inicial, FOV, ra;
    float l, r, b, t;
    float distancia, alcada_max;
    int alcadaModel;
    float radiEsc, movz, franjes;
       
    glm::vec3 VRP;
    glm::vec3 maxim_escena, minim_escena;


    // VAO i VBO names
    GLuint VAO_Patr, VBO_PatrPos, VBO_PatrNorm, VBO_PatrMatamb, VBO_PatrMatdiff, VBO_PatrMatspec, VBO_PatrMatshin;
    GLuint VAO_Terra, VBO_TerraPos, VBO_TerraNorm, VBO_TerraMatamb, VBO_TerraMatdiff, VBO_TerraMatspec, VBO_TerraMatshin;
    QGLShaderProgram *program;

    
    float minx, miny, minz, maxx, maxy, maxz;
    float escalaPatr1, escalaPatr2, escalaPatr3;
    
    GLuint transLoc, projLoc, viewLoc;
    GLuint vertexLoc, normalLoc, matambLoc, matdiffLoc, matspecLoc, matshinLoc, colorfocusLoc, franjesLoc;
    Model patr;
    // paràmetres calculats a partir de la capsa contenidora del model
    glm::vec3 centreBasePatr;

    typedef  enum {NONE, ROTATE} InteractiveAction;
    InteractiveAction DoingInteractive;
    int xClick, yClick;
    float angleX, angleY;
};
