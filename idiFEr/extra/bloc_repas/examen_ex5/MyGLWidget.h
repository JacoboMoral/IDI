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

  public slots:
    void changeZoom(int angle);
    void focusEsc();
    void focusCam();
    
  signals:
    void zoom_signal(int angle);
    void activaRadioCam ();
    void activaRadioEsc ();
    
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
    void createBuffers ();
    void bufferModel ();
    void bufferVaca ();
    void bufferTerra ();
    void carregaShaders ();
    void projectTransform ();
    void viewTransform ();
    void modelTransformTerra ();
    void modelTransformPatricio ();
    void modelTransformVaca ();
    void calculaCapsaModel ();
    void initialize_Camera ();
    void ratllarPantalla ();
    void cambiarFocus ();
    void ini_focus ();
    
    // VAO i VBO names
    GLuint VAO_Patr, VBO_PatrPos, VBO_PatrNorm, VBO_PatrMatamb, VBO_PatrMatdiff, VBO_PatrMatspec, VBO_PatrMatshin;
    GLuint VAO_Terra, VBO_TerraPos, VBO_TerraNorm, VBO_TerraMatamb, VBO_TerraMatdiff, VBO_TerraMatspec, VBO_TerraMatshin;
    
    GLuint VAO_Vaca, VBO_VacaPos, VBO_VacaNorm;
    GLuint VBO_VacaMatamb, VBO_VacaMatdiff, VBO_VacaMatspec, VBO_VacaMatshin;
    // Program
    QOpenGLShaderProgram *program;
    // uniform locations
    GLuint transLoc, projLoc, viewLoc;
    // attribute locations
    GLuint vertexLoc, normalLoc, matambLoc, matdiffLoc, matspecLoc, matshinLoc;

    // model
    Model patr, Vaca;
    // paràmetres calculats a partir de la capsa contenidora del model
    glm::vec3 centrePatr, centreVaca;
    float escala, esc_vaca;
    // radi de l'escena
    float radiEsc;
    
    float minyv;
    float minx, miny, minz, maxx, maxy, maxz;
	float FOV_inicial, FOV, ra;
	glm::vec3 VRP;
	bool ratllat, focuscamara;
	GLuint ratllatLoc, focuscamaraLoc, posfocusLoc, colorfocusLoc;
	glm::vec3 posFocus; // Posicio del focus
	glm::vec3 colFocus; //Color del focus


    typedef  enum {NONE, ROTATE, ZOOM} InteractiveAction;
    InteractiveAction DoingInteractive;
    int xClick, yClick;
    float angleY, angleX;
    float rotar;
    bool perspectiva;
};

