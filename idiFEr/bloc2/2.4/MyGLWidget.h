#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "model.h"
//#include <QLineEdit> // CAL????

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();

  public slots:
    void changeZoom(int angle);
    void chargePatricio();
    void chargePatricio2();
    void chargeLego();
    void chargeHomer();
    void setPerspective();
    void setOrthogonal();
    void setEscala(int esc);
    void eulerX(int graus);
    void eulerY(int graus);
    void changeTerraColor(int r_new, int g_new, int b_new);
    void viewportW(int prop);
    void viewportH(int prop);
    
  signals:
    void angleX(int x);
    void angleY(int y);
    
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
    void legoTransform ();
    void homerTransform ();
    void modelTransform_Terra ();
    void projectTransform ();
    void viewTransform ();
    // initialize_Camera - Aqui incloem les inicialitzacions de la camara.
    void initialize_Camera ();
    
    // update_Camera - Aqui incloem les actualitzacions de la camara.
    //void update_Camera ();
    
    // bufferModel - Per tal de generar automaticament el buffer dun nou model.
    void bufferModel ();
    void bufferTerra ();
    void bufferLego ();
    void bufferHomer ();
    
    // calcul_Contenidora - Calcula punt maxim minim i radi de la caixa contenidora
    void calcul_Contenidora();
    void repintar();
    void calcul_Escena();
        
    
    // mousePressEvent - Es cridat quan es prem una boto del ratoli
    virtual void mousePressEvent (QMouseEvent *e);
    
    // mouseMoveEvent - Es cridat quan es mou el ratoli
    virtual void mouseMoveEvent(QMouseEvent *e);

    // attribute locations
    GLuint vertexLoc, colorLoc;
    // uniform locations
    GLuint transLoc, viewLoc, projLoc;
    // VAO i VBO names
    GLuint VAO_Model, VBO_ModelPos, VBO_ModelCol;
    GLuint VAO_Lego, VBO_LegoPos, VBO_LegoCol;
    GLuint VAO_Homer, VBO_HomerPos, VBO_HomerCol;
    GLuint VAO_Terra, VBO_Terra, VBO_TerraCol;
    // Program
    QOpenGLShaderProgram *program;
    // Internal vars
    float scale, FOV_inicial, FOV, ra, Znear, Zfar, rotar, radi_escena, radi_model, costat_terra, distancia;
    float puntX_click, puntY_click, rotate_x, rotate_y;
    float l, r, b, t, fPatesc_y, fLegesc_y, fHomesc_y, escalat;
    bool optica, num_model, num_model2;
    float red, green, blue;
    float w_paint, h_paint;
    glm::vec3 pos, OBS, VRP, up, centre_esfera, centre_base, centre_base2, centre_base3;
    glm::vec3 puntmax, puntmin;
    Model m, m2, m3;

};

