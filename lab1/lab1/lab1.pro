TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    Class.cc \
    Student.cc \
    main.cc

HEADERS += \
    Class.h \
    Student.h

DISTFILES += \
    BugReport.txt \
    Classes.txt \
    Students.txt
