QT += qml quick widgets

TEMPLATE = app

LIBS += -lgit2

SOURCES += \
    main.cpp \
    githandler.cpp \
    gitrepository.cpp \
    repositorymodel.cpp \
    gitreflog.cpp \
    gitbranch.cpp \
    gitreference.cpp \
    gitbase.cpp \
    gitoid.cpp

HEADERS += \
    githandler.h \
    gitrepository.h \
    repositorymodel.h \
    gitreflog.h \
    gitbranch.h \
    gitreference.h \
    gitbase.h \
    gitoid.h

DISTFILES += \
    qml/MainView.qml

RESOURCES += \
    resources.qrc
