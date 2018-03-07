#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
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

  private:
    void createBuffers ();
    void carregaShaders ();
    void modelTransform ();
    void modelTransform_Terra ();
    void projectTransform ();
    void viewTransform ();
    // initialize_Camera - Aqui incloem les inicialitzacions de la camara.
    void initialize_Camera ();
    
    // bufferModel - Per tal de generar automaticament el buffer dun nou model.
    void bufferModel ();
    void bufferTerra ();
    
    // calcul_Contenidora - Calcula punt maxim minim i radi de la caixa contenidora
    void calcul_Contenidora();

    // attribute locations
    GLuint vertexLoc, colorLoc;
    // uniform locations
    GLuint transLoc, viewLoc, projLoc;
    // VAO i VBO names
    GLuint VAO_Model, VBO_ModelPos, VBO_ModelCol;
    GLuint VAO_Terra, VBO_Terra, VBO_TerraCol;
    // Program
    QOpenGLShaderProgram *program;
    // Internal vars
    float scale, ra, Znear, Zfar, rotar, FOV_inicial, FOV, radi, distancia;
    glm::vec3 pos, OBS, VRP, up, centre_esfera;
    glm::vec3 puntmax, puntmin;
    Model m;

};

