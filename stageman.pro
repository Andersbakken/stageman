######################################################################
# Automatically generated by qmake (2.01a) Fri Dec 11 21:54:25 2009
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += stagemodel.h \
           stageobject.h \
           stagescene.h \
           stageview.h \
           window.h \
           acteditor.h
SOURCES += main.cpp \
           stagemodel.cpp \
           stageobject.cpp \
           stagescene.cpp \
           stageview.cpp \
           window.cpp \
           acteditor.cpp
CONFIG += debug
unix {
    MOC_DIR=.moc
    UI_DIR=.ui
    OBJECTS_DIR=.obj
} else {
    MOC_DIR=tmp/moc
    UI_DIR=tmp/ui
    OBJECTS_DIR=tmp/obj
}
