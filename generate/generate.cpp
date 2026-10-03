#include <iostream>
#include <vector>
#include <string>
#include "../model/Room.h"
using namespace std;


// Global variables from main.cpp

extern vector<Subject> subjects;
extern vector<Teacher> teachers;
extern vector<Section> sections;
extern vector<string> slots;
extern vector<vector<Class>> timetable;
extern vector<Room> rooms;
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

bool roomBusy(int roomId, int slot){
    for (int i = 0; i < sections.size(); i++) {
        if (timetable[i][slot].roomId == roomId) {
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


bool sectionBusy(int section, int slot) {
    if (timetable[section][slot].subjectId != -1) {
        return true;
    }

    return false;
}


// Currently Handeling 3 conditions
// 1. No teacher found
// 2. Teacher already teaching another section
// 3. Subject cannot occupy same slot twice
bool isValid(int section, int slot, int subjectId, int roomId)
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

    // Room already occupied
    if (roomBusy(roomId, slot)) {
        return false;
    }

    // Subject cannot occupy same slot twice
    if (subjectAlreadyUsed(section, slot, subjectId)) {
        return false;
    }

    return true;
}
bool generate(int section, int slot){
    if (section == sections.size()) {
        return true;
    }
    if (slot == slots.size()) {
        return generate(section + 1, 0);
    }

   for (int i = 0; i < subjects.size(); i++) {

    int subjectId = subjects[i].id;
    int teacherId = findTeacher(subjectId);

    // Try every room
    for (int j = 0; j < rooms.size(); j++) {

        int roomId = rooms[j].id;

        if (isValid(section, slot, subjectId, roomId)) {

            timetable[section][slot].subjectId = subjectId;
            timetable[section][slot].teacherId = teacherId;
            if(subjects[i].duration > 1){
                for(int j=1;j<subjects[i].duration;j++){
                    timetable[section][slot+j].subjectId = subjectId;
                    timetable[section][slot+j].teacherId = teacherId;
                }
            }
            if (generate(section, slot + subjects[i].duration)) {
                return true;
            }

            // Backtrack
            timetable[section][slot].subjectId = -1;
            timetable[section][slot].teacherId = -1;
            if(subjects[i].duration > 1){
                for(int j=1;j<subjects[i].duration;j++){
                    timetable[section][slot+j].subjectId = -1;
                    timetable[section][slot+j].teacherId = -1;
                }
            }
        }
    }

    return false;
}
