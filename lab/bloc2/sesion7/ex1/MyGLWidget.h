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

    // mouse events 
    virtual void mousePressEvent   (QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent *event);
    virtual void mouseMoveEvent    (QMouseEvent *event);
    
  private:
    void createBuffers ();
    void carregaShaders ();
    void modelTransform ();
    void terraTransform ();
    void viewTransform ();
    void projectTransform ();
    void inicialitzaCamera ();
    void radiEsferaContenidora ();
    
    // attribute locations
    GLuint vertexLoc, colorLoc;
    // uniform locations
    GLuint transLoc, viewLoc, projectaLoc;

    // VAO i VBO names
    GLuint VAO,       VBO_vertexs, VBO_color;
    GLuint VAO_terra, VBO_terraV,  VBO_terraC;

    // Program
    QOpenGLShaderProgram *program;

    // Model
    Model model;
    
    // Internal vars
    float escale;
    glm::vec3 pos;

    // Optica de la camera
    glm::vec3 obs, vrp, vup;
    glm::vec3 centre;
    float fov, deltaFov, ra, znear, zfar;
    float fovi, rViewport;
    float radi;

    // angles d'Euler
    float theta, psi, phi;
    float deltaA;
    glm::vec3 eixX, eixY, eixZ;

    // Interaccio amb cursor
    typedef enum {NOINTERACCIO, ROTACIO} Interaccio;
    Interaccio interaccio;
    int        xClick, yClick;
};
