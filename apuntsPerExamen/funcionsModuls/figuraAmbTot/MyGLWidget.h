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
  
  public slots:
	void canviarASCO(bool value);
    void canviarASCA(bool value);
    void zoomSlider(int value);
    void rotarPatr(int value);
    
signals:
    void changeToSCO();
    void changeToSCA();
    void emitZoom(int value);
    void emitRotar(int value);


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

  private:
    void createBuffersModel ();
    void createBuffersTerra ();
    void carregaShaders ();
    void projectTransform ();
    void viewTransform ();
    void modelTransformIdent ();
    void modelTransformModel ();
    void modelTransformModelLego ();
    void modelTransformModelLego2 ();
    void modelTransformModelLego3();
    void modelTransformModelLego4 ();
    void modelTransformModelLego5 ();
    void modelTransformModelLego6 ();
    void modelTransformModelLego7 ();
    void modelTransformModelLego8 ();
    void modelTransformModelLego9 ();
    void modelTransformModelLego10 ();
    void modelTransformModelLego11 ();
    void modelTransformModelLego12 ();
    void modelTransformModelLego13 ();
    void modelTransformModelLego14 ();
    void modelTransformModelLego15 ();
    void canviarFocus();
    void calculaCapsaModel (Model &p, float &escala, glm::vec3 &CentreBase);
    
    
    void calculs_noretall();
    void initialize_Camera();


    // VAO i VBO names
    GLuint VAO_Model, VBO_ModelPos, VBO_ModelNorm, VBO_ModelMatamb, VBO_ModelMatdiff, VBO_ModelMatspec, VBO_ModelMatshin;
    GLuint VAO_Terra, VBO_TerraPos, VBO_TerraNorm, VBO_TerraMatamb, VBO_TerraMatdiff, VBO_TerraMatspec, VBO_TerraMatshin;
    // Program
    QOpenGLShaderProgram *program;
    // uniform locations
    GLuint transLoc, projLoc, viewLoc;
    // attribute locations
    GLuint vertexLoc, normalLoc, matambLoc, matdiffLoc, matspecLoc, matshinLoc, boolFocusLoc;

    // model
    Model m;
    // paràmetres calculats a partir de la capsa contenidora del model
    glm::vec3 centreBaseModel;
    float escalaModel;
    // radi de l'escena
    float radiEsc;

    typedef  enum {NONE, ROTATE} InteractiveAction;
    InteractiveAction DoingInteractive;
    int xClick, yClick;
    float angleY, angleX;
    bool perspectiva;
    bool segonfocus;
    float eix_x, eix_z;
    float rotar;
    
    float FOV_inicial, FOV, ra;
    float l, r, b, t;
    float distancia, alcada_max;
    int alcadaModel;
       
    glm::vec3 VRP;
    glm::vec3 maxim_escena, minim_escena;

};

