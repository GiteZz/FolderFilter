#ifndef HANDLESETTINGS_H
#define HANDLESETTINGS_H
#include <QString>
#include<vector>


class handleSettings
{
public:
    handleSettings();

    std::vector<QString> handleSettings::getLocationFiles(QString settingsFileLocation);
};

#endif // HANDLESETTINGS_H
