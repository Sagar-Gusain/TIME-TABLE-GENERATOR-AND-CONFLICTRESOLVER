#include <iostream>
#include <vector>
#include <string>

#include "model/Subjects.h"
#include "model/Teacher.h"
#include "model/Sections.h"
#include "model/Class.h"
#include "model/Room.h"

using namespace std;

//--------Refrence------------//
// timetable taking row=sections and col=slots
// timetable[section][slot]
//              8-9       9-10      10-11      11-12
//            ┌─────────┬─────────┬─────────┬─────────┐
// Section A  │  Maths  │   DSA   │  DBMS   │   OS    │
//            ├─────────┼─────────┼─────────┼─────────┤
// Section B  │   DSA   │  Maths  │   OS    │  DBMS   │
//            ├─────────┼─────────┼─────────┼─────────┤
// Section C  │  DBMS   │   OS    │  Maths  │   DSA   │
//            └─────────┴─────────┴─────────┴─────────┘
//--------EndRefrence--------//


// Find teacher who teaches the subject
int findTeacher(int subjectId){
    for (int i = 0; i < teachers.size(); i++) {
        if (teachers[i].subjectId == subjectId) {
            return teachers[i].id;
        }
    }
    return -1;
}

// Check whether teacher is already teaching
bool teacherBusy(int teacherId, int slot){
    for (int i = 0; i < sections.size(); i++) {
        if (timetable[i][slot].teacherId == teacherId) {
            return true;
        }
    }
    return false;
}

// Check whether subject is already present
bool subjectAlreadyUsed(int section,int slot, int subjectId){
    for (int i = 0; i < slots.size(); i++) {
        if (i != slot && timetable[section][i].subjectId == subjectId) {
            return true;
        }
    }
    return false;
}


// Currently Handeling 3 conditions
// 1. No teacher found
// 2. Teacher already teaching another section
// 3. Subject cannot occupy same slot twice
bool isValid(int section, int slot, int subjectId)
{
    int teacherId = findTeacher(subjectId);
    // No teacher found
    if (teacherId == -1) {
        return false;
    }
    // Teacher already teaching another section
    if (teacherBusy(teacherId, slot)) {
        return false;
    }
    // Subject cannot occupy same slot twice
    if (subjectAlreadyUsed(section, slot, subjectId)) {
        return false;
    }
    return true;
}

bool generate(int section, int slot){
    // Try every subject
    for (int i = 0; i < subjects.size(); i++) {

        int subjectId = subjects[i].id;
        if (isValid(section, slot, subjectId)) {
            int teacherId = findTeacher(subjectId);

            timetable[section][slot].subjectId = subjectId;
            timetable[section][slot].teacherId = teacherId;

            if (generate(section, slot + 1)) {
                return true;
            }
            // Backtrack
            timetable[section][slot].subjectId = -1;
            timetable[section][slot].teacherId = -1;
        }
    }

    return false;
}
