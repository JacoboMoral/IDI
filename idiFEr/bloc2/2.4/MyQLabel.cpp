#include "MyQLabel.h"
#include <iostream>

using namespace std;

MyQLabel::MyQLabel(QWidget *parent) : QLabel(parent)
{
    //Incialitzacio, si cal, dels atributs
    red = green = blue = 0;
}

void MyQLabel::setRed(int r)
{
    //cout << "RED: " << r << endl;
    red = r;
    setColors();
}

void MyQLabel::setGreen(int g)
{
    //cout << "GREEN: " << g << endl;
    green = g;
    setColors();
}

void MyQLabel::setBlue(int b)
{
    //cout << "BLUE: " << b << endl;
    blue = b;
    setColors();
}

void MyQLabel::setColors() {
    this->setStyleSheet(QString("MyQLabel {background-color: rgb(%1,%2, %3);}").arg(red).arg(green).arg(blue));
    
	emit setTerraColor(red, green, blue);
}
