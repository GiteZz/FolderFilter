#ifndef HOTKEY_H
#define HOTKEY_H
#include "mainwindow.h"


class Hotkey
{
public:
    Hotkey();

private:
    void handleHotKey(QList<int> keys,MainWindow *w);
    int isNumber(int key);
};

#endif // HOTKEY_H
