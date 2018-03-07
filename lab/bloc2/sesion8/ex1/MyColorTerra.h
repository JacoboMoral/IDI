/*
* Paleta de colors MyColorTerra.h
*
* Widget que hereta de QLabel i que
* cal incloure'l en  MyForm.ui amb promote
*/

#define GLM_FORCE_RADIANS

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QLabel>
#include <QColor>
#include <QString>

class MyColorTerra : public QLabel
{
    Q_OBJECT

    public:
    MyColorTerra (QWidget *parent);
    // ~MyColorTerra ();

    public slots:
    void setRoig (int);
    void setVerd (int);
    void setBlau (int);

    signals:
    void sendColorTerra(int, int, int);

    private:  // global variables
    int roig, verd, blau;
};
