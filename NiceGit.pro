QT += qml quick widgets

TEMPLATE = app

LIBS += -lgit2

SOURCES += \
    main.cpp \
    githandler.cpp \
    gitrepository.cpp \
    repositorymodel.cpp

HEADERS += \
    githandler.h \
    gitrepository.h \
    repositorymodel.h

DISTFILES += \
    qml/MainView.qml

DEPLOYMENT += $$DISTFILES

RESOURCES += \
    resources.qrc
