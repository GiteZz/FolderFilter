#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QKeyEvent>
#include <QMediaPlayer>

#include "structs.h"
#include <qvideowidget.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool eventFilter(QObject *object, QEvent *event);

private slots:
    void on_addFolderButton_clicked();
    void setNameSlot(QWidget *PB);
    void setLocationSlot(QWidget *PB);

    void on_nameComboBox_currentIndexChanged(int index);
    
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent *event);

    bool setImage(QString path);

    void handleKeys();

    std::vector<int> containsNumber();

    int isNumber(int key);

    void on_locationComboBox_currentIndexChanged(int index);

    void on_volumeSlider_sliderMoved(int position);

    void on_saveButton_clicked();

    void on_saveNextButton_clicked();

    void on_keepButton_clicked();

    void on_timeSlider_sliderMoved(int position);

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

    int currentNameIndex;
    int currentLocIndex;

    bool fillingNameCombobox;
    bool fillingLocCombobox;

    bool fileSet;
    
    QList<int> currentKeys;

    int getFileType(QString filename);
    void setFile(int index);

    QMediaPlayer *player;
    QVideoWidget *videoWidget;

    bool setAudio(QString path);
    bool setVideo(QString path);

    std::vector<int> allowableNames;
    std::vector<int> allowableLocations;

    void saveFile();
    bool removeFile(QString filePath);
};

#endif // MAINWINDOW_H
