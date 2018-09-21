QT += widgets
QT += xml
QT += core

HEADERS       = analogclock.h \
    cdata.h
SOURCES       = analogclock.cpp \
                main.cpp \
    cdata.cpp

QMAKE_PROJECT_NAME = widgets_analogclock

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/widgets/analogclock
INSTALLS += target
