#ifndef SUBJECT_H
#define SUBJECT_H

#include <string>
using namespace std;

struct Subject {
    int id;
    int teacherId;
    int sectionId;
    int lecturesPerWeek;
    string name;
    bool requiresLab;
};

#endif