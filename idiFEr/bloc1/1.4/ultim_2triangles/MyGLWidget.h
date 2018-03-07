#define GLM_FORCE_RADIANS

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"


class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();
    
  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ();

    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ();
 
    // resize - Es cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);  
    
    virtual void keyPressEvent (QKeyEvent *e);
    void modelTransform ();
    void modelTransform2 ();

  private:
    void createBuffers ();
    void createBuffers2 ();
    void carregaShaders ();
    
    // attribute locations
    GLuint vertexLoc, varLoc, transLoc, transLoc2;
    
    // Program
    QOpenGLShaderProgram *program;

    GLuint VAO, VBO, VAO2, VBO2;
    
    float factor_escalat = 1, x = 0, y = 0, angle = 0, escalax = 1, escalay = 1;
};
