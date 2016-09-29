QT += qml quick widgets

TEMPLATE = app

LIBS += -lgit2

SOURCES += \
    main.cpp \
    githandler.cpp \
    gitrepository.cpp

HEADERS += \
    githandler.h \
    gitrepository.h

DISTFILES += \
    qml/MainView.qml

DEPLOYMENT += $$DISTFILES

RESOURCES += \
    resources.qrc
