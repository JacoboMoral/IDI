#include <QLabel>
#include <QString>
class myLabel: public QLabel
{
  Q_OBJECT
  public:
  	myLabel(QWidget *parent);
	bool haMogut;
	int size;
	
  public slots:
	void truncar(int);
};
 
