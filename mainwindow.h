#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
struct nameStruct{
    std::vector<QString> catNames;
    std::vector<std::vector<bool> > catAllowed;
    std::vector<std::vector<QString> > names;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_addFolderButton_clicked();
    void setNameSlot(QWidget *PB);
    void setLocationSlot(QWidget *PB);

    void on_nameComboBox_currentIndexChanged(int index);

private:

    Ui::MainWindow *ui;
    std::vector<QString> filepaths;
    void openFolder();
    void setNameVertLay();
    void setLocVertLay();
    struct nameStruct readFile(QString location);
    void writeSaveFiles();

    struct nameStruct name_struct;
    struct nameStruct location_struct;

    QList<QPushButton *> *namePBList;
    QList<QPushButton *> *locationPBList;
    QList<QLineEdit *> *nameLEList;
    QList<QLineEdit *> *locationLEList;
    void setNameComBox();
    void setLocComBox();

    int currentIndex;

    bool fillingCombobox;
};

#endif // MAINWINDOW_H
