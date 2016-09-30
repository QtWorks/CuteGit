QT += qml quick widgets

TEMPLATE = app

LIBS += -lgit2

SOURCES += \
    main.cpp \
    githandler.cpp \
    gitrepository.cpp \
    repositorymodel.cpp \
    gitreflog.cpp

HEADERS += \
    githandler.h \
    gitrepository.h \
    repositorymodel.h \
    gitreflog.h

DISTFILES += \
    qml/MainView.qml

RESOURCES += \
    resources.qrc
