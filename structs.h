#ifndef STRUCTS_H
#define STRUCTS_H
#include <QString>
#include <vector>

struct nameStruct{
    std::vector<QString> catNames;
    std::vector<std::vector<bool> > catAllowed;
    std::vector<std::vector<QString> > names;
};


#endif // STRUCTS_H
