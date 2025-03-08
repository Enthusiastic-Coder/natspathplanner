
TEMPLATE = app
CONFIG += console
CONFIG += c++17

SOURCES += aircraft.cpp
HEADERS += aircraft.h

# Later: Add radarwindow.cpp, scenario_manager.cpp, etc. here

SOURCES += radarwindow.cpp \
            scenariomanager.cpp \
            trainingscorer.cpp

HEADERS += radarwindow.h \
            scenariomanager.h \
            trainingscorer.h

SOURCES += holdingpattern.cpp \
            holdingpatternmanager.cpp

HEADERS += holdingpattern.h \
            holdingpatternmanager.h

SOURCES += radarwindow.cpp
HEADERS += radarwindow.h

SOURCES += spacinglogic.cpp
HEADERS += spacinglogic.h
