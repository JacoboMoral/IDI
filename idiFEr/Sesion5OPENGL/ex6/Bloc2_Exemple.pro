TEMPLATE    = app
QT         += opengl 

INCLUDEPATH += /usr/include/glm /dades/daniel.angel.perez/IDI/Model

FORMS += MyForm.ui

HEADERS += MyForm.h MyGLWidget.h

SOURCES += main.cpp MyForm.cpp \
        MyGLWidget.cpp  /dades/daniel.angel.perez/IDI/Model/model.cpp
