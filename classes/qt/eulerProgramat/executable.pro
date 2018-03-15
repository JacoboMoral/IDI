QT           += opengl
    
CONFIG       += qt
TEMPLATE      = app

DEPENDPATH   +=.
INCLUDEPATH  += /usr/include/glm    \
               ./Model

HEADERS      += MyEuler.h

SOURCES      += main.cpp    \
                MyEuler.cpp \
                ./Model/model.cpp
