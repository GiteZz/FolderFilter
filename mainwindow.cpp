#include "mainwindow.h"
#include "handlesettings.h"
#include "ui_mainwindow.h"
#include "structs.h"
#include "enums.h"

#include <QDirIterator>
#include <QFileDialog>
#include <QDebug>
#include <QSignalMapper>
#include <QObject>
#include <QKeyEvent>
#include <QtWidgets>
#include <qvideowidget.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug() << "Started in: " << QDir::currentPath();


    std::vector<QString> locationFiles = handleSettings::getInfoFromTag(QDir::currentPath() + "/settings.txt","<location>");
    std::vector<QString> nameFiles = handleSettings::getInfoFromTag(QDir::currentPath() + "/settings.txt","<name>");

    location_struct = handleSettings::readFile(locationFiles);
    name_struct =  handleSettings::readFile(nameFiles);

    currentNameIndex = 0;
    currentLocIndex = 0;

    fileSet = false;

    nameLEList = new QList<QLineEdit *>;
    namePBList = new QList<QPushButton *>;
    locationLEList = new QList<QLineEdit *>;
    locationPBList = new QList<QPushButton *>;

    player = new QMediaPlayer();

    ui->volumeSlider->setValue(50);
    videoWidget = new QVideoWidget;

    ui->videoLayout->addWidget(videoWidget);

    player->setVideoOutput(videoWidget);

    videoWidget->hide();
}

MainWindow::~MainWindow()
{
    delete nameLEList;
    delete namePBList;
    delete locationLEList;
    delete locationPBList;
    delete ui;
}


void MainWindow::openFolder(){
    qDebug() << "openFolder()";

    QString filePath = QFileDialog::getExistingDirectory(this,
                                                       tr("Open folder"),
                                                       "",
                                                       QFileDialog::ShowDirsOnly |
                                                       QFileDialog::DontResolveSymlinks);
    qDebug() << "Adding Files from: " << filePath;


    QDirIterator dirIt(filePath,QDirIterator::Subdirectories);
    while (dirIt.hasNext())
    {
        dirIt.next();
        if (QFileInfo(dirIt.filePath()).isFile())
        {
            if (getFileType(dirIt.filePath()) >= 0)
            {
                filepaths.push_back(dirIt.filePath());
            }
        }
    }

    qDebug() << "Filling uiListWidget";
    for(int i = 0; i < filepaths.size(); i++){
        ui->itemListWidget->addItem(QFileInfo(filepaths.at(i)).fileName());
    }

    //TEMP
    if(!fileSet){
        qDebug() << "no file set, setting first file";
        setFile(0);
    }

    qDebug() << "Filled uiListWidget";
    qDebug() << "~openFolder()";
}

void MainWindow::on_addFolderButton_clicked()
{
    qDebug() << "addFolderButton clicked";
    openFolder();
}

void MainWindow::setNameVertLay(){
    qDebug() << "setNameVertLay()";

    while(ui->nameVerticalLayout->count() != 0){
        QLayoutItem *item = ui->nameVerticalLayout->takeAt(0);
        delete item->widget();
        delete item;
    }
    while(ui->namePBVerticalLayout->count() != 0){
        QLayoutItem *item = ui->namePBVerticalLayout->takeAt(0);
        delete item->widget();
        delete item;
    }

    nameLEList->clear();
    namePBList->clear();

    for(int i = 0; i < name_struct.names.at(allowableNames.at(currentNameIndex)).size(); i++){
        qDebug() << "dd";
        QPushButton *add = new QPushButton("+");
        ui->namePBVerticalLayout->addWidget(add);

        QSignalMapper * mapper = new QSignalMapper(this);
        QObject::connect(mapper,SIGNAL(mapped(QWidget *)),this,SLOT(setNameSlot(QWidget *)));

        QObject::connect(add, SIGNAL(clicked(bool)),mapper,SLOT(map()));
        mapper->setMapping(add, add);

        qDebug() << "everything mapped";

        namePBList->append(add);

        QLineEdit *addLine = new QLineEdit(name_struct.names.at(allowableNames.at(currentNameIndex)).at(i));
        ui->nameVerticalLayout->addWidget(addLine);
        nameLEList->append(addLine);

    }

    qDebug() << "~setNameVertLay()";
}

void MainWindow::setLocVertLay(){
    qDebug() << "setLocVertLay()";

    //Clear the layouts
    while(ui->locationVerticalLayout->count() != 0){
        QLayoutItem *item = ui->locationVerticalLayout->takeAt(0);
        delete item->widget();
        delete item;
    }
    while(ui->locationPBVerticalLayout->count() != 0){
        QLayoutItem *item = ui->locationPBVerticalLayout->takeAt(0);
        delete item->widget();
        delete item;
    }

    //Clear the lists
    locationLEList->clear();
    locationPBList->clear();
    qDebug() << "cleared";
    for(int i = 0; i < location_struct.names.at(allowableLocations.at(currentLocIndex)).size(); i++){
        qDebug() << "dd";
        QPushButton *add = new QPushButton("+");
        ui->locationPBVerticalLayout->addWidget(add);

        QSignalMapper * mapper = new QSignalMapper(this);
        QObject::connect(mapper,SIGNAL(mapped(QWidget *)),this,SLOT(setLocationSlot(QWidget *)));

        QObject::connect(add, SIGNAL(clicked(bool)),mapper,SLOT(map()));
        mapper->setMapping(add, add);

        qDebug() << "everything mapped";

        locationPBList->append(add);

        QLineEdit *addLine = new QLineEdit(location_struct.names.at(allowableLocations.at(currentLocIndex)).at(i));
        ui->locationVerticalLayout->addWidget(addLine);
        locationLEList->append(addLine);

    }

    qDebug() << "~setLocVertLay()";
}

void MainWindow::writeSaveFiles(){
    qDebug() << "writeSaveFiles()";

    QFile nametxt("names.txt");
    QTextStream inName(&nametxt);
    if ( nametxt.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        for(int i = 0; i < name_struct.catNames.size(); i++){
            inName<< name_struct.catNames.at(i) + " - ";
            if(name_struct.catAllowed.at(i).at(0))inName<< "image";
            if(name_struct.catAllowed.at(i).at(1))inName<< "music";
            if(name_struct.catAllowed.at(i).at(2))inName<< "video";
            inName<< "{\r\n";
            for(int a = 0; a < name_struct.names.at(i).size(); a++){
                inName<< name_struct.names.at(i).at(a) + "\r\n";
            }
            inName<< "}\r\n";
        }

        nametxt.flush();
        nametxt.close();
    }else qDebug() << "Failed to open in order to save";
    nametxt.flush();
    nametxt.close();

    QFile locationtxt("locations.txt");
    QTextStream inLocation(&locationtxt);
    if ( nametxt.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        for(int i = 0; i < name_struct.catNames.size(); i++){
            inLocation<< name_struct.catNames.at(i) + " - ";
            if(name_struct.catAllowed.at(i).at(0))inLocation<< "image";
            if(name_struct.catAllowed.at(i).at(1))inLocation<< "music";
            if(name_struct.catAllowed.at(i).at(2))inLocation<< "video";
            inLocation<< "{\r\n";
            for(int a = 0; a < name_struct.names.at(i).size(); a++){
                inLocation<< name_struct.names.at(i).at(a) + "\r\n";
            }
            inLocation<< "}\r\n";
        }

        nametxt.flush();
        nametxt.close();
    }else qDebug() << "Failed to open in order to save";
    locationtxt.flush();
    locationtxt.close();

    qDebug() << "~writeSaveFiles()";
}

void MainWindow::setNameSlot(QWidget *PB){
    qDebug() << "setNameSlot()";

    int index = namePBList->indexOf((QPushButton *)PB);

    ui->completeNameLineEdit->setText(ui->completeNameLineEdit->text() + nameLEList->at(index)->text());
    qDebug() << "~setNameSlot()";
}

void MainWindow::setLocationSlot(QWidget *PB){
    qDebug() << "setLocationSlot()";

    int index = locationPBList->indexOf((QPushButton *)PB);
    ui->completeLocationLineEdit->setText(locationLEList->at(index)->text());

    QFont font("", 0);
    QFontMetrics fm(font);

    int height = ui->completeLocationLineEdit->height();
    int width = fm.width(locationLEList->at(index)->text());
    qDebug() << ui->completeNameLineEdit->geometry().y();
    ui->completeNameLineEdit->move(ui->completeLocationLineEdit->geometry().x() + width + 10,ui->completeNameLineEdit->geometry().y());
    ui->completeLocationLineEdit->setFixedSize(width, height);

    qDebug() << "~setLocationSlot()";
}

void MainWindow::setNameComBox(){
   qDebug() << "setNameComBox()";
   fillingNameCombobox = true;
   for(int i = 0; i < allowableNames.size(); i++){
    ui->nameComboBox->addItem(name_struct.catNames.at(allowableNames.at(i)));
   }
   fillingNameCombobox = false;
    qDebug() << "~setNameComBox()";
}

void MainWindow::setLocComBox(){
   qDebug() << "setLocComBox";
   fillingLocCombobox = true;
  for(int i = 0; i < allowableLocations.size(); i++){
    ui->locationComboBox->addItem(location_struct.catNames.at(allowableLocations.at(i)));
  }
  fillingLocCombobox = false;
  qDebug() << "~SetLocComBox";
}

void MainWindow::on_nameComboBox_currentIndexChanged(int index)
{
    if(!fillingNameCombobox){
        currentNameIndex = index;
        setNameVertLay();
    }
}

bool MainWindow::setImage(QString path){
    QPixmap image(path);
    ui->imageLabel->setPixmap(image);
}

bool MainWindow::setAudio(QString path){
    qDebug() << "setAudio()";
    player->setMedia(QUrl::fromLocalFile(path));
    player->setVolume(ui->volumeSlider->value());
    player->play();
    qDebug() << "~setAudio()";
}

bool MainWindow::setVideo(QString path){
    player->setMedia(QUrl::fromLocalFile(path));
    videoWidget->show();
    player->play();
}

void MainWindow::keyPressEvent(QKeyEvent * event){
    /*
    currentKeys.append(event->key());
    qDebug() << currentKeys;
    if(currentKeys.contains(CONTROL)){
        if(currentKeys.contains(KEY_K)){
            on_keepButton_clicked();
        }else if(currentKeys.contains(KEY_N)){
            on_saveNextButton_clicked();
        }else if(currentKeys.contains(KEY_S)){
            on_saveButton_clicked();
        }else if(currentKeys.contains(KEY_O)){
            openFolder();
        }
    }
    */

    qDebug("At least came here");
        QString text = event->text();
        qDebug() << text;
}

void MainWindow::keyReleaseEvent(QKeyEvent *event){
    currentKeys.removeAt(currentKeys.indexOf(event->key()));
}

void MainWindow::handleKeys(){
    std::vector<int> numbers = containsNumber();
    if(currentKeys.contains(SHIFT) && numbers.size()>0){
        for(int i = 0; i < numbers.size(); i++){
            
        }
    }
}

int MainWindow::isNumber(int key){
    switch (key)
    {
    case 38:
        return 0;
    case 201:
        return 1;
    case 34:
        return 2;
    case 39:
        return 3;
    case 40:
        return 4;
    case 167:
        return 5;
    case 200:
        return 6;
    case 33:
        return 7;
    case 199:
        return 8;
    case 192:
        return 9;
    case 48:
        return 9;
    }
    if(key>=49 && key<=57)return key-49;
    return -1;
}

std::vector<int> MainWindow::containsNumber(){
    std::vector<int> ret;
    for(int i = 0; i < currentKeys.length(); i++){
        if(isNumber(currentKeys.at(i))>=0)ret.push_back(isNumber(currentKeys.at(i)));
    }
    return ret;
}

//Clear everything, set file, doesn't delete anything, deleting should be handled by other function
void MainWindow::setFile(int index){
    videoWidget->hide();
    player->stop();
    ui->imageLabel->clear();
    ui->timeSlider->setValue(0);
    //Check if image/sound/video/..
    if(index >= filepaths.size()){
        qDebug() << "No next file available";
    }else{
        int fileType = getFileType(filepaths.at(index));

        allowableLocations.clear();
        allowableNames.clear();
        //Calculate allowable types image/music/video
        for(int i = 0; i < name_struct.catAllowed.size(); i++){
            if(name_struct.catAllowed.at(i).at(fileType)){
                allowableNames.push_back(i);
            }
        }

        for(int i = 0; i < location_struct.catAllowed.size(); i++){
            if(location_struct.catAllowed.at(i).at(fileType)){
                allowableLocations.push_back(i);
            }
        }

        if(fileType == -1){
            qDebug() << "Wrong filetype! Shouldn't happen";
        }else if (fileType == 0){
            //Image file
            setImage(filepaths.at(index));
        }else if (fileType == 1){
            //Image file
            setAudio(filepaths.at(index));
        }else if(fileType == 2){
            //Video file
            setVideo(filepaths.at(index));
        }

        setNameVertLay();
        setNameComBox();
        setLocVertLay();
        setLocComBox();
    }
}

//Determines filetype, gives positive if valid type, otherwise return -1;
int MainWindow::getFileType(QString filename){
    if(QFileInfo(filename).suffix() == "bmp" ||
            QFileInfo(filename).suffix() == "jpg" ||
            QFileInfo(filename).suffix() == "png" ||
            QFileInfo(filename).suffix() == "jpeg"||
            QFileInfo(filename).suffix() == "JPG" ||
            QFileInfo(filename).suffix() == "gif"){

        return 0;
    }else if(QFileInfo(filename).suffix() == "mp3"){

        return 1;
    }else if(QFileInfo(filename).suffix() == "avi" ||
             QFileInfo(filename).suffix() == "mp4"){

        return 2;
    }else return -1;
}

void MainWindow::on_locationComboBox_currentIndexChanged(int index)
{
    if(!fillingLocCombobox){
        currentLocIndex = index;
        setLocVertLay();
    }
}

void MainWindow::on_volumeSlider_sliderMoved(int position)
{
    player->setVolume(position);
}

void MainWindow::saveFile(){
    QFile f(filepaths.at(0));
    f.open(QIODevice::ReadOnly);

    QFileInfo in = QFileInfo(filepaths.at(0));
    QString ext = in.suffix();
    QString name = ui->completeNameLineEdit->text();
    QString path = ui->completeLocationLineEdit->text();
    QString saveName;
    if (path[path.length()-1]=='/')
    {
        saveName = path + name + "." + ext;
    }
    else
    {
        saveName = path + "/" + name + "." + ext;
    }
    f.copy(saveName);


    //f.remove(filepaths.at(currentImageIndex));
    f.close();
}

void MainWindow::on_saveButton_clicked()
{
    saveFile();
    ui->completeLocationLineEdit->setText("");
    ui->completeNameLineEdit->setText("");
}

void MainWindow::on_saveNextButton_clicked()
{
    saveFile();
    ui->completeLocationLineEdit->setText("");
    ui->completeNameLineEdit->setText("");
    qDebug() << removeFile(filepaths.at(0));

    delete ui->itemListWidget->takeItem(0);
    filepaths.erase(filepaths.begin());
    setFile(0);

}

bool MainWindow::removeFile(QString filePath){
    QFile f(filePath);
    return f.remove();
}

void MainWindow::on_keepButton_clicked()
{
    ui->completeLocationLineEdit->setText("");
    ui->completeNameLineEdit->setText("");

    delete ui->itemListWidget->takeItem(0);
    filepaths.erase(filepaths.begin());
    setFile(0);
}

void MainWindow::on_timeSlider_sliderMoved(int position)
{
    qint64 dur = player->duration();
    player->setPosition((position/100.0)*dur);
}


bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (object == this && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_W) {
            return true;
        } else {
            return false;
        }
    }
    return false;
}
