#include <QApplication>
#include <QQmlEngine>
#include <QQuickView>
#include <QQmlContext>

#include <githandler.h>
#include <gitrepository.h>
#include <repositorymodel.h>
#include <gitbranch.h>
#include <commitmodel.h>
#include <graphpoint.h>
#include <commitgraph.h>

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQuickView view;

    qmlRegisterUncreatableType<CommitModel>("org.semlanik.nicegit", 1, 0, "CommitModel", "Owned only by GitHandler");
    qmlRegisterUncreatableType<CommitGraph>("org.semlanik.nicegit", 1, 0, "CommitGraph", "Owned only by GitHandler");
    qmlRegisterUncreatableType<GraphPoint>("org.semlanik.nicegit", 1, 0, "GraphPoint", "Owned only by GitHandler");
    qmlRegisterUncreatableType<RepositoryModel>("org.semlanik.nicegit", 1, 0, "RepositoryModel", "Owned only by GitHandler");
    qmlRegisterUncreatableType<GitRepository>("org.semlanik.nicegit", 1, 0, "GitRepository", "Owned only by GitHandler");
    qmlRegisterUncreatableType<GitBranch>("org.semlanik.nicegit", 1, 0, "GitRepository", "Owned only by GitHandler");
    qmlRegisterUncreatableType<GitHandler>("org.semlanik.nicegit", 1, 0, "GitHandler", "Global for qml");


    GitHandler handler;
    handler.open("/home/semlanik/Projects/HCAT/hmi_hcat/demo/default/HCAT/");
    view.rootContext()->setContextProperty("_handler", &handler);
    view.setSource(QUrl("qrc:/qml/MainView.qml"));
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.showMaximized();
    return app.exec();
}
