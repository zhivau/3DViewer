# Основная конфигурация
TEMPLATE = app  # Компилируем как приложение
CONFIG += c++17 console
QT -= gui

# Источники тестов
SOURCES += \
           filereaderTest.cpp \
           transformTest.cpp \
           ../model/fileReader.cpp \
           ../model/transformMatrix.cpp

# Заголовочные файлы
HEADERS += ../model/fileReader.h \
            ../model/baseObjects.h \
            ../model/scene.h \
            ../model/transformMatrix.h

# Подключение GoogleTest
LIBS += -lgtest -lgtest_main -pthread -lstdc++fs

# Компиляторские флаги
QMAKE_CXXFLAGS += -std=c++17 -Wall -Wextra -Werror
