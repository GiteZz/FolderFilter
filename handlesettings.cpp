#include "handlesettings.h"

#include "structs.h"
#include <QDebug>
#include <QTextStream>
#include <QFile>
#include <QString>
#include <vector>

handleSettings::handleSettings()
{

}

std::vector<QString> handleSettings::getInfoFromTag(QString settingsFileLocation,QString tag){
    qDebug() << "getLocationFiles()";

    QFile file(settingsFileLocation);
   if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
       qDebug() << "Couldn't open settings fille";
       return {};

        }else {

       qDebug() << "Opening settingsfile for location files";
       std::vector<QString> retVec;
       QTextStream in(&file);
       while(!in.atEnd()){
           QString line = in.readLine();
           if(line == tag){
               while(!in.atEnd()){
                   line = in.readLine();
                   if(line.contains('<') || line == ""){
                       //End of locations
                       break;
                   }else{
                      retVec.push_back(line);
                   }
               }
           }
       }
       return retVec;
   }
}


struct nameStruct handleSettings::readFile(std::vector<QString> location){
    qDebug() << "readFile()";
    nameStruct fileStruct;
    for(int i = 0; i < location.size(); i++){
    QFile file(location.at(i));

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Failed to open file: " << location.at(i);
    }

    QTextStream in(&file);

    // true if inside categorie, used for checking if safefile is valid
    bool catOpened = false;


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
    }
    qDebug() << "~readFile()";
    return fileStruct;

}
