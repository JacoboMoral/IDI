#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "model.h" //clase model




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
    void modelTransform2 (); 
    void projectTransform ();
    void viewTransform ();
    //void ini_camera ();

    //interaccion para verctor de up con angulo i tecla
    //float vupx, vupy, alfa;
    
    //variables de la camara
    //float FOV, raw, znear, zfar;
    //glm::vec3 OBS, VRP, up;
    
    //Parametres del perpective
    //Relacio de aspecte del window, relacio de aspecte del viweport
    //FOV que voy modificando cuando modifico la pantalla, y el FOVi 
    //que es el fov inicial que es el que utilizo para la ecuacion 
    float raw, rav, FOV, FOVi;
    
    float alfa;
    // attribute locations
    GLuint vertexLoc, colorLoc;
    // uniform locations
    GLuint transLoc, projLoc, viewLoc;
    // VAO i VBO names
    GLuint VAO_Homer, VBO_vertexs, VBO_color;
    
    GLuint VAO_Terra, VBO_posterra, VBO_TerraCol;   
    // Program
    QOpenGLShaderProgram *program;
    // Internal vars
    float scale;
    glm::vec3 pos;
    Model model;
};

