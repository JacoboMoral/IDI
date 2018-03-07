#include <QLabel>

class MyQLabel: public QLabel
{
    Q_OBJECT
    
    public:
        MyQLabel(QWidget *parent);
        void setColors();

    public slots:
        void setRed(int r);
        void setGreen(int g);
        void setBlue(int b);
        
    signals:
        void setTerraColor(int red, int green, int blue);
        
    private:
        int red, green, blue;
        
};
