#ifndef HANDLESETTINGS_H
#define HANDLESETTINGS_H
#include <QString>
#include<vector>


class handleSettings
{
public:
    handleSettings();

    static std::vector<QString> getInfoFromTag(QString settingsFileLocation,QString tag);
    static struct nameStruct readFile(std::vector<QString> location);
};

#endif // HANDLESETTINGS_H
