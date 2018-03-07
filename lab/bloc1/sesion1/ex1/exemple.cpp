#include <QApplication>
#include <QPushButton>

int main(int argc, char **argv){
    QApplication a(argc,argv);
    QPushButton hello("holiholii!",0);
    hello.resize(500,300);
    hello.show();
    return a.exec();
}
