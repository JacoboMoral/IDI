#define GLM_FORCE_RADIANS 

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include "glm/glm.hpp"
#include <QKeyEvent>
#include "glm/gtc/matrix_transform.hpp"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();
    
  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ();
    virtual void keyPressEvent (QKeyEvent *event);

    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ();
 
    // resize - Es cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);  

  private:
    void createBuffers ();
    void carregaShaders ();
    void transformaModel ();
    
    // attribute locations
    GLuint vertexLoc;
    
    // Program
    QOpenGLShaderProgram *program;

    GLuint VAO, VBO;
    
    float  ex, ey, ez;  // factors d'escala
    GLuint TGLoc;       // uniform location
    
};
