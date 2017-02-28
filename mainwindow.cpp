#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDirIterator>
#include <QFileDialog>
#include <QDebug>
#include <QSignalMapper>
#include <QObject>
#include <QKeyEvent>

enum keys{
    SHIFT=16777248,
    CONTROL=16777249,
    ALT=16777251,
    NKEY=78,
    LKEY=76
};


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug() << "Started in: " << QDir::currentPath();
    name_struct = readFile(QDir::currentPath() + "/names.txt");
    location_struct = readFile(QDir::currentPath() + "/locations.txt");


    //Only debug
    for(int i = 0; i < name_struct.catNames.size(); i++){
        qDebug() << name_struct.catNames.at(i);
        qDebug() << "categories: " << " " << name_struct.catAllowed.at(i).at(0) << " " << name_struct.catAllowed.at(i).at(1) << " " << name_struct.catAllowed.at(i).at(2);
        for(int x = 0; x < name_struct.names.at(i).size(); x++){
            qDebug() << "  -" << name_struct.names.at(i).at(x);
        }
    }

    currentIndex = 0;

    nameLEList = new QList<QLineEdit *>;
    namePBList = new QList<QPushButton *>;
    locationLEList = new QList<QLineEdit *>;
    locationPBList = new QList<QPushButton *>;

    setNameVertLay();
    setNameComBox();
    setLocVertLay();
    setLocComBox();
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
            if (QFileInfo(dirIt.filePath()).suffix() == "bmp" ||
                QFileInfo(dirIt.filePath()).suffix() == "mp3" ||
                QFileInfo(dirIt.filePath()).suffix() == "jpg" ||
                QFileInfo(dirIt.filePath()).suffix() == "png" ||
                QFileInfo(dirIt.filePath()).suffix() == "jpeg" ||
                QFileInfo(dirIt.filePath()).suffix() == "avi" ||
                QFileInfo(dirIt.filePath()).suffix() == "gif")
            {
                filepaths.push_back(dirIt.filePath());
            }
        }
    }

    qDebug() << "Filling uiListWidget";
    for(int i = 0; i < filepaths.size(); i++){
        ui->itemListWidget->addItem(QFileInfo(filepaths.at(i)).fileName());
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

    for(int i = 0; i < name_struct.names.at(currentIndex).size(); i++){
        qDebug() << "dd";
        QPushButton *add = new QPushButton("+");
        ui->namePBVerticalLayout->addWidget(add);

        QSignalMapper * mapper = new QSignalMapper(this);
        QObject::connect(mapper,SIGNAL(mapped(QWidget *)),this,SLOT(setNameSlot(QWidget *)));

        QObject::connect(add, SIGNAL(clicked(bool)),mapper,SLOT(map()));
        mapper->setMapping(add, add);

        qDebug() << "everything mapped";

        namePBList->append(add);

        QLineEdit *addLine = new QLineEdit(name_struct.names.at(currentIndex).at(i));
        ui->nameVerticalLayout->addWidget(addLine);
        nameLEList->append(addLine);

    }

    qDebug() << "~setNameVertLay()";
}

void MainWindow::setLocVertLay(){
    qDebug() << "setLocVertLay()";

    while(ui->locationVerticalLayout->count() != 0){
        QLayoutItem *item = ui->locationVerticalLayout->takeAt(0);
        delete item->widget();
        delete item;
    }
    while(ui->locationPBVerticalLayout->count() != 0){
        QLayoutItem *item = ui->locationVerticalLayout->takeAt(0);
        delete item->widget();
        delete item;
    }

    locationLEList->clear();
    locationPBList->clear();

    for(int i = 0; i < location_struct.names.at(currentIndex).size(); i++){
        qDebug() << "dd";
        QPushButton *add = new QPushButton("+");
        ui->locationPBVerticalLayout->addWidget(add);

        QSignalMapper * mapper = new QSignalMapper(this);
        QObject::connect(mapper,SIGNAL(mapped(QWidget *)),this,SLOT(setLocationSlot(QWidget *)));

        QObject::connect(add, SIGNAL(clicked(bool)),mapper,SLOT(map()));
        mapper->setMapping(add, add);

        qDebug() << "everything mapped";

        locationPBList->append(add);

        QLineEdit *addLine = new QLineEdit(location_struct.names.at(currentIndex).at(i));
        ui->locationVerticalLayout->addWidget(addLine);
        locationLEList->append(addLine);

    }

    qDebug() << "~setLocVertLay()";
}

struct nameStruct MainWindow::readFile(QString location){
    qDebug() << "readFile()";
    QFile file(location);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Failed to open file: " << location;
    }

    QTextStream in(&file);

    // true if inside categorie, used for checking if safefile is valid
    bool catOpened = false;

    nameStruct fileStruct;
    std::vector<QString> nameFiller;
    std::vector<bool> catFiller;


    while(!in.atEnd()){
        QString line = in.readLine();
        if(line.contains('{') && !catOpened){
            catOpened = true;
            catFiller.clear();
            QString name = line.section('-',0,0).simplified();
            fileStruct.catNames.push_back(name);

            QString cat = line.section('-',1,1);

            catFiller.push_back(cat.contains("image"));
            catFiller.push_back(cat.contains("music"));
            catFiller.push_back(cat.contains("video"));

            if(cat.contains("all")){
                for(int i = 0; i < 3; i++)catFiller.at(i) = 1;
            }

            fileStruct.catAllowed.push_back(catFiller);

        }else if(line.contains('{') && catOpened)
            qDebug() << "Categorie opened before closing!";

        else if(line.contains('}')){
            catOpened = false;
            fileStruct.names.push_back(nameFiller);
            nameFiller.clear();
        }
        else if(line.count() != 0)nameFiller.push_back(line.simplified());
    }
    qDebug() << "~readFile()";
    return fileStruct;

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
   fillingCombobox = true;
   for(int i = 0; i < name_struct.catNames.size(); i++){
    ui->nameComboBox->addItem(name_struct.catNames.at(i));
   }
   fillingCombobox = false;
    qDebug() << "~setNameComBox()";
}

void MainWindow::setLocComBox(){
   qDebug() << "setLocComBox";
  for(int i = 0; i < location_struct.catNames.size(); i++){
    ui->locationComboBox->addItem(location_struct.catNames.at(i));
  }
  qDebug() << "~SetLocComBox";
}

void MainWindow::on_nameComboBox_currentIndexChanged(int index)
{
    if(!fillingCombobox){
        currentIndex = index;
        setNameVertLay();
    }
}

void MainWindow::setImage(QString path){
    //ui->imageLabel->set
}

void MainWindow::keyPressEvent(QKeyEvent * event){
    currentKeys.append(event->key());
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
