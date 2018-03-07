/*
 * textEco  RJA setembre 2016
 *
 * main.cpp
 */

#include <QApplication>
#include <QFrame>
#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>

int main(int argc, char *argv[]){

  QApplication app(argc, argv);

  // CreaciÃ³ de la interfÃ­cie grÃ fica
  QFrame      *w           = new QFrame();
  QVBoxLayout *mainLayout  = new QVBoxLayout(w);

  QFont font("Times", 15, QFont::Bold,true);

  QVBoxLayout *textLayout  = new QVBoxLayout();
  QLabel      *labelEntrat = new QLabel ("Edita un text :", w);
  labelEntrat->setFont(font);
  QLineEdit   *textEntrat  = new QLineEdit(w);
  QLabel      *labelEco    = new QLabel ("Text replicat", w);
  labelEco->setFont(font);
  QTextEdit   *textEco     = new QTextEdit(w);
  textEco->setReadOnly(true);

  textLayout->addWidget(labelEntrat);
  textLayout->addWidget(textEntrat);
  textLayout->addWidget(labelEco);
  textLayout->addWidget(textEco);

  QHBoxLayout *buttonsLayout = new QHBoxLayout();
  QPushButton *netejaButton  = new QPushButton("&Neteja", w);
  netejaButton->setStyleSheet("background-color:cyan;");
  QPushButton *sortirButton   = new QPushButton("&Sortir", w);
  sortirButton->setStyleSheet("background-color:red;");
  QSpacerItem *quitSpacer = new QSpacerItem(30, 10,
                                            QSizePolicy::Expanding,
                                            QSizePolicy::Minimum);

  buttonsLayout->addWidget(netejaButton);
  buttonsLayout->addItem(quitSpacer);
  buttonsLayout->addWidget(sortirButton);

  mainLayout->addLayout(textLayout);
  QSpacerItem *vertSpacer = new QSpacerItem(30, 60,
                                            QSizePolicy::Expanding,
                                            QSizePolicy::Minimum);
  mainLayout->addItem(vertSpacer);
  mainLayout->addLayout(buttonsLayout);

  QWidget::connect(textEntrat, SIGNAL(textChanged(const QString &)),
                   textEco,    SLOT(setText(const QString &)));
  QWidget::connect(netejaButton, SIGNAL(clicked()),
                   textEntrat,   SLOT(clear()));
  QWidget::connect(sortirButton, SIGNAL(clicked()),
                   w,      SLOT(close()));

  // execucio

  w->resize(800,200);
  w->show();
  return app.exec();
}
