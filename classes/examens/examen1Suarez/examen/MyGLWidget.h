#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QMouseEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "model.h"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();

  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ( );
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ( );
    // resizeGL - És cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);
    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);
    // mouse{Press/Release/Move}Event - Són cridades quan es realitza l'event 
    // corresponent de ratolí
    virtual void mousePressEvent (QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent *event);
    virtual void mouseMoveEvent (QMouseEvent *event);
    virtual void wheelEvent ( QWheelEvent * event );
  private:
    void createBuffers ();
    void carregaShaders ();
    void projectTransform ();
    void viewTransform ();
    void modelTransformTerra ();
    void modelTransformPatricio1 ();
    void modelTransformPatricio2 ();
    void modelTransformCow ();
    void calculaCapsaModel ();
    void calculs_noretall();
    void initialize_Camera();
    void calculaCapsaModelCow();
    void initFocus();
    
    
    
    
    float FOV_inicial, FOV, ra;
    float l, r, b, t;
    float distancia, alcada_max;
    int alcadaModel;
       
    glm::vec3 VRP;
    glm::vec3 maxim_escena, minim_escena;


    
    
    
    // VAO i VBO names
    GLuint VAO_Patr, VBO_PatrPos, VBO_PatrNorm, VBO_PatrMatamb, VBO_PatrMatdiff, VBO_PatrMatspec, VBO_PatrMatshin;
    GLuint VAO_cow, VBO_cowPos, VBO_cowNorm, VBO_cowMatamb, VBO_cowMatdiff, VBO_cowMatspec, VBO_cowMatshin;
    GLuint VAO_Terra, VBO_TerraPos, VBO_TerraNorm, VBO_TerraMatamb, VBO_TerraMatdiff, VBO_TerraMatspec, VBO_TerraMatshin;
    // Program
    QOpenGLShaderProgram *program;
    // uniform locations
    GLuint transLoc, projLoc, viewLoc, posFocusLoc;
    // attribute locations
    GLuint vertexLoc, normalLoc, matambLoc, matdiffLoc, matspecLoc, matshinLoc;

      float minx, miny, minz, maxx, maxy, maxz;
      float minxcow, minycow, minzcow, maxxcow, maxycow, maxzcow;

      float movx, movz;
    // model
    Model patr, cow;
    // paràmetres calculats a partir de la capsa contenidora del model
    glm::vec3 centrePatr, centrecow;
    float escala, escalacow;
    // radi de l'escena
    float radiEsc,rotar;

    typedef  enum {NONE, ROTATE} InteractiveAction;
    InteractiveAction DoingInteractive;
    int xClick, yClick;
    float angleY, angleX;
    bool perspectiva;
};

