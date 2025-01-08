unix:!macx {
    # Для Linux
    QT += core gui opengl widgets  # Добавляем модули для Linux
}

# Условие для Windows
win32: {
    LIBS += -lopengl32  # Подключаем OpenGL библиотеку на Windows
    QT += core gui opengl widgets openglwidgets  # Добавляем модуль openglwidgets для Windows
}

# Условие для macOS
macx {
    QT += core gui opengl widgets openglwidgets  # Добавляем модуль openglwidgets для macOS
}


include(QtGifImage/gifimage/qtgifimage.pri)
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QMAKE_CXXFLAGS += -Wall -Wextra -Werror


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    model/fileReader.cpp \
    model/transformMatrix.cpp \
    view/widget.cpp \
    view/glview.cpp \
    model/drawerscene.cpp \
    view/command.cpp \
    model/facade.cpp


HEADERS += \
    model/baseObjects.h \
    model/fileReader.h \
    model/scene.h \
    model/transformMatrix.h \
    view/glview.h \
    view/widget.h \
    view/drawerscene.h \
    view/controller.h \
    view/command.h \
    model/facade.h

QMAKE_CXXFLAGS += -DGL_SILENCE_DEPRECATION

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

