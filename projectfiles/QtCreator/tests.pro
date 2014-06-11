QT += testlib
CONFIG += c++11
TEMPLATE = app
TARGET = tests
INCLUDEPATH +=

# Input
HEADERS += \
           ../../tests/catch.hpp \
           ../../src/av/ringbuffer.hpp

SOURCES += ../../tests/testringbuffer.cpp \
           ../../tests/main.cpp \

