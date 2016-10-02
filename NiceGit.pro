QT += qml quick widgets

TEMPLATE = app

LIBS += -lgit2

SOURCES += \
    main.cpp \
    githandler.cpp \
    gitrepository.cpp \
    repositorymodel.cpp \
    gitbranch.cpp \
    gitreference.cpp \
    gitbase.cpp \
    gitoid.cpp \
    gitcommit.cpp \
    commitmodel.cpp \
    universallistmodel.cpp

HEADERS += \
    githandler.h \
    gitrepository.h \
    repositorymodel.h \
    gitbranch.h \
    gitreference.h \
    gitbase.h \
    gitoid.h \
    gitcommit.h \
    commitmodel.h \
    universallistmodel.h

RESOURCES += \
    resources.qrc

debug {
    DEFINES += DEBUG
}
