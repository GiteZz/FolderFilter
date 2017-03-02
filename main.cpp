#include "mainwindow.h"
#include <QApplication>
#include <QtWidgets/QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QEvent>

class QApplicationFilter: public QObject
{
    public:
        QApplicationFilter(): QObject() {}
        ~QApplicationFilter() {}

        bool eventFilter(QObject* obj, QEvent* event)
        {
            ;
            if(event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease){
            qDebug() << "QApplicationFilter: "
                     << obj->objectName()
                     << " - event type: "
                     << event->type();

                int key = static_cast<QKeyEvent*>(event)->key();
                qDebug() << key;
            }
            return QObject::eventFilter(obj, event);
        }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.installEventFilter(new QApplicationFilter());
    MainWindow w;
    w.show();

    return a.exec();
}
