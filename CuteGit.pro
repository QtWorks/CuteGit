QT += qml quick widgets concurrent

QMAKE_CXXFLAGS = -std=c++14

TARGET = CuteGit

TEMPLATE = app

unix {
    desktop.files = CuteGit.desktop
    desktop.path = $$SYSROOT/usr/share/applications
    INSTALLS += desktop
    LIBS += -lgit2
}

win32 {
    INCLUDEPATH += $$PWD//include
    LIBS += -L$$PWD//lib// -lgit2
}

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
    diffmodel.cpp \
    settings.cpp \
    universallistmodelbase.cpp \
    gitauthenticator.cpp \
    notificationcenter.cpp \
    notification.cpp

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
    diffmodel.h \
    settings.h \
    universallistmodelbase.h \
    gitauthenticator.h \
    notificationcenter.h \
    notification.h

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    CuteGit.desktop
debug {
    DEFINES += DEBUG
}

target.path = $$SYSROOT/usr/bin
INSTALLS += target
