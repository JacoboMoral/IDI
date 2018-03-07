#include "MyLabel.h"

//constructor

MyLabel::MyLabel(QWidget *parent) : QLabel(parent) {
  //incialitzacio, si cal, dels atributs
  r = g = b = 0;
}

//implementacio slots
void MyLabel::verd(int a){
  g = a;
  this->setStyleSheet(QString("MyLabel {background-color: rgb(%1,%2,%3);}").arg(r).arg(g).arg(b));
  emit color(r,g,b);
}

void MyLabel::vermell(int a){
  r = a;
  this->setStyleSheet(QString("MyLabel {background-color: rgb(%1,%2,%3);}").arg(r).arg(g).arg(b));
  emit color(r,g,b);
}

void MyLabel::blau(int a){
  b = a;
  this->setStyleSheet(QString("MyLabel {background-color: rgb(%1,%2,%3);}").arg(r).arg(g).arg(b));
  emit color(r,g,b);
}

