#include "mainwindow.h"
#include <QApplication>
#include <QtWidgets/QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QEvent>
#include <QObject>
#include <QWindow>
#include <QMetaClassInfo>
#include "hotkey.h"

class QApplicationFilter: public QObject
{
    public:
        QApplicationFilter(MainWindow *mwP): QObject() {
            w = mwP;
        }
        ~QApplicationFilter() {}

        QList<int> keyPressed;
        MainWindow *w;
        bool eventFilter(QObject* obj, QEvent* event)
        {

            if(event->type() == QEvent::KeyPress && (QString)obj->metaObject()->className() == "QWidgetWindow" && !static_cast<QKeyEvent*>(event)->isAutoRepeat()){
                //&& obj->metaObject()->className()== QWidgetWindow)
                qDebug() << obj->metaObject()->className();
                if(!keyPressed.contains(static_cast<QKeyEvent*>(event)->key())){
                    keyPressed.append(static_cast<QKeyEvent*>(event)->key());
                    qDebug() << "Key pressed!";
                    qDebug() << keyPressed;
                    w->handleHotKey(keyPressed);
                }
            }

            if(event->type() == QEvent::KeyRelease && (QString)obj->metaObject()->className() == "QWidgetWindow" && !static_cast<QKeyEvent*>(event)->isAutoRepeat()){
                qDebug() << "================Key released!";
                qDebug() << obj->metaObject()->className();
                if(keyPressed.contains(static_cast<QKeyEvent*>(event)->key())){
                    keyPressed.erase(keyPressed.begin() + keyPressed.indexOf(static_cast<QKeyEvent*>(event)->key()));
                }
                qDebug() << keyPressed;
            }
            return QObject::eventFilter(obj, event);
        }
};

Hotkey *hKey;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *w = new MainWindow();
    a.installEventFilter(new QApplicationFilter(w));

    w->show();

    return a.exec();
}
