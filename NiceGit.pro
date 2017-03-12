QT += qml quick widgets concurrent

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
    universallistmodel.cpp \
    commitgraph.cpp \
    graphpoint.cpp \
    gittag.cpp \
    tagmodel.cpp \
    gitdiff.cpp \
    graphlistmodel.cpp \
    gitremote.cpp \
    gitbaseoid.cpp \
    branchlistmodel.cpp \
    taglistmodel.cpp \
    tooltipviewmodel.cpp \
    gitconsole.cpp \
    colorhandler.cpp \
    diffmodel.cpp

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
    universallistmodel.h \
    commitgraph.h \
    graphpoint.h \
    gittag.h \
    tagmodel.h \
    gitdiff.h \
    graphlistmodel.h \
    gitremote.h \
    gitbaseoid.h \
    branchlistmodel.h \
    taglistmodel.h \
    tooltipviewmodel.h \
    gitconsole.h \
    colorhandler.h \
    diffmodel.h

RESOURCES += \
    resources.qrc

debug {
    DEFINES += DEBUG
}
