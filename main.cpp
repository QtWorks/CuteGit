#include <QApplication>
#include <QQmlEngine>
#include <QQuickView>
#include <QQmlContext>

#include <githandler.h>
#include <gitrepository.h>
#include <repositorymodel.h>

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQuickView view;

    qmlRegisterUncreatableType<GitHandler>("org.semlanik.nicegit", 1, 0, "GitHandler", "Global for qml");
    qmlRegisterUncreatableType<GitRepository>("org.semlanik.nicegit", 1, 0, "GitRepository", "Owned only by GitHandler");
    qmlRegisterUncreatableType<RepositoryModel>("org.semlanik.nicegit", 1, 0, "RepositoryModel", "Owned only by GitHandler");

    GitHandler handler;
    handler.open("/home/semlanik/Projects/testrepo/");
    view.rootContext()->setContextProperty("_handler", &handler);
    view.setSource(QUrl("qrc:/qml/MainView.qml"));
    view.showMaximized();
    return app.exec();
}
