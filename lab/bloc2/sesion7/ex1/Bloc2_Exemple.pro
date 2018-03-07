TEMPLATE     = app
QT          += opengl 

INCLUDEPATH += /usr/include/glm    \
               /assig/idi/Model

FORMS       += MyForm.ui

HEADERS     += MyForm.h    \
               MyGLWidget.h

SOURCES     += main.cpp       \
               MyForm.cpp     \
               MyGLWidget.cpp \
               /assig/idi/Model/model.cpp
