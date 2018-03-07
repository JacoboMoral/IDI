#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Model/model.h"

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
    virtual void mousePressEvent (QMouseEvent *event);
    virtual void mouseMoveEvent (QMouseEvent *event);
    
  private:
    void createBuffers ();
    void carregaShaders ();
    void modelTransform ();
    void modelTransform1 ();
    void modelTransformTerra ();
    void projectTransform();
    void viewTransform();
    void ini_camera();
    void calc_capsacont();

    // attribute locations
    GLuint vertexLoc, colorLoc;
    // uniform locations
    GLuint transLoc, projLoc, viewLoc;
    // VAO i VBO names
    GLuint VAO_Pat, VAO_Terra, VBO_Terra, VBO_PatPos, VBO_PatCol;
    // Program
    QOpenGLShaderProgram *program;
    // Internal vars
    float scale, fov, f, ra, znear, zfar, rot, radi, rotx, roty, left, right, top, bottom;
    int clicx, clicy;
    glm::vec3 pos, obs, vrp, up, centre, cbase;
    bool axon;
    Model m;
};

