#include <QApplication>
#include <QQmlEngine>
#include <QQuickView>
#include <QQmlContext>

#include <githandler.h>

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQuickView view;

    qmlRegisterUncreatableType<GitHandler>("org.semlanik.nicegit", 1, 0, "GitHandler", "Global for qml");
    GitHandler handler;
    view.rootContext()->setContextProperty("_handler", &handler);
    view.setSource(QUrl("qrc:/qml/MainView.qml"));
    view.showMaximized();
    return app.exec();
}
