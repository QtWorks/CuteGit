#include <QApplication>
#include <QQmlEngine>
#include <QQuickView>
#include <QQmlContext>
#include <QFontDatabase>

#include <githandler.h>
#include <gitconsole.h>
#include <gitrepository.h>
#include <repositorymodel.h>
#include <gitbranch.h>
#include <commitmodel.h>
#include <graphpoint.h>
#include <commitgraph.h>
#include <gittag.h>
#include <gitdiff.h>
#include <gitoid.h>
#include <tooltipviewmodel.h>
#include <diffmodel.h>

#include <graphlistmodel.h>
#include <branchlistmodel.h>
#include <taglistmodel.h>

#include <QDebug>
static TooltipViewModel* ttmodel = nullptr;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QFontDatabase::addApplicationFont(":/fonts/Inconsolata.otf");
    QQuickView view;

    qmlRegisterUncreatableType<GitOid>("org.semlanik.nicegit", 1, 0, "GitOid", "Owned only by GitHandler");
    qmlRegisterUncreatableType<CommitModel>("org.semlanik.nicegit", 1, 0, "CommitModel", "Owned only by GitHandler");
    qmlRegisterUncreatableType<CommitGraph>("org.semlanik.nicegit", 1, 0, "CommitGraph", "Owned only by GitHandler");
    qmlRegisterUncreatableType<GraphPoint>("org.semlanik.nicegit", 1, 0, "GraphPoint", "Owned only by GitHandler");
    qmlRegisterUncreatableType<RepositoryModel>("org.semlanik.nicegit", 1, 0, "RepositoryModel", "Owned only by GitHandler");
    qmlRegisterUncreatableType<GitRepository>("org.semlanik.nicegit", 1, 0, "GitRepository", "Owned only by GitHandler");
    qmlRegisterUncreatableType<GitBranch>("org.semlanik.nicegit", 1, 0, "GitBranch", "Owned only by GitHandler");
    qmlRegisterUncreatableType<GitHandler>("org.semlanik.nicegit", 1, 0, "GitHandler", "Global for qml");
    qmlRegisterUncreatableType<GitTag>("org.semlanik.nicegit", 1, 0, "GitTag", "Global for qml");
    qmlRegisterUncreatableType<GitDiff>("org.semlanik.nicegit", 1, 0, "GitDiff", "Global for qml");
    qmlRegisterUncreatableType<GitCommit>("org.semlanik.nicegit", 1, 0, "GitCommit", "Global for qml");
    qmlRegisterUncreatableType<GraphListModel>("org.semlanik.nicegit", 1, 0, "GraphListModel", "Owned only by GitHandler");
    qmlRegisterUncreatableType<BranchListModel>("org.semlanik.nicegit", 1, 0, "BranchListModel", "Owned only by GitHandler");
    qmlRegisterUncreatableType<TagListModel>("org.semlanik.nicegit", 1, 0, "TagListModel", "Owned only by GitHandler");
    qmlRegisterUncreatableType<GitConsole>("org.semlanik.nicegit", 1, 0, "GitConsole", "Owned only by GitHandler");
    qmlRegisterUncreatableType<DiffModel>("org.semlanik.nicegit", 1, 0, "DiffModel", "Owned only by GitHandler");
    qmlRegisterSingletonType<TooltipViewModel>("org.semlanik.nicegit", 1, 0,"TooltipViewModel",
                                               [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject*
    {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        if(!ttmodel) {
            ttmodel = new TooltipViewModel;
        }
        return ttmodel;
    });

    GitHandler handler;
    handler.open("/home/semlanik/Projects/HCAT/hmi_hcat/demo/default/HCAT/");
    view.rootContext()->setContextProperty("_handler", &handler);
    view.setSource(QUrl("qrc:/qml/MainView.qml"));
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.showMaximized();
    return app.exec();
}
