#include "handlesettings.h"
#include <QTextStream>
#include <QFile>
#include <QString>

handleSettings::handleSettings()
{

}

std::vector<QString> handleSettings::getLocationFiles(QString settingsFileLocation){
    QFile file(settingsFileLocation);
   if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
       qDebug() << "Couldn't open settings fille";
       return new std::vector<QString>;
   }else {
       qDebug << "Opening settingsfile for location files";
       std::vector<QString> retVec = new std::vector<QString>;
       QTextStream in(&file);
       while(!in.atEnd()){
           QString line = in.readLine();
           if(line == "<locations>"){
               while(!in.atEnd()){
                   line = in.readLine();
                   if(line.contains('<') || line != ""){
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
