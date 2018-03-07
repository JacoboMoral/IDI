#include <QLabel>

class MyLabel: public QLabel
{
  Q_OBJECT

  public:

    MyLabel (QWidget *parent);

  public slots:

    void verd(int a);
    void blau(int a);
    void vermell(int a);

  signals:
    
    void color(int a, int b, int c);
    
  private:
    
    int r,g,b;

};