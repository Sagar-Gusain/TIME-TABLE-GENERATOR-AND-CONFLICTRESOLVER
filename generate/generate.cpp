#include <iostream>
#include <vector>
#include <string>
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


// Currently Handling conditions:
// 1. Teacher exists
// 2. Subject not already assigned in this section
// 3. For all slots across the subject duration:
//    - Slot is within bounds
//    - Slot is free (no break and not already occupied)
//    - Teacher is free
//    - Room is free
bool isValid(int section, int slot, int subjectId, int roomId, int duration = 1)
{
    int teacherId = findTeacher(subjectId);

    // No teacher found
    if (teacherId == -1) {
        return false;
    }

    // Subject cannot occupy same section twice
    if (subjectAlreadyUsed(section, slot, subjectId)) {
        return false;
    }

    for (int d = 0; d < duration; d++) {
        int s = slot + d;

        // Slot out of bounds
        if (s >= slots.size()) {
            return false;
        }

        // Slot is already occupied or is a break
        if (timetable[section][s].subjectId != -1) {
            return false;
        }

        // Teacher already teaching another section
        if (teacherBusy(teacherId, s)) {
            return false;
        }

        // Room already occupied
        if (roomBusy(roomId, s)) {
            return false;
        }
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
    
    // If current slot is already occupied (break or previous assignment), proceed
    if (timetable[section][slot].subjectId != -1) {
        return generate(section, slot + 1);
    }
    
    for (int i = 0; i < subjects.size(); i++) {

        int subjectId = subjects[i].id;
        int teacherId = findTeacher(subjectId);
        int duration = subjects[i].duration;

        if (slot + duration > slots.size()) {
            continue;
        }

        // Try every room
        for (int j = 0; j < rooms.size(); j++) {

            int roomId = rooms[j].id;

            if (isValid(section, slot, subjectId, roomId, duration)) {

                for (int d = 0; d < duration; d++) {
                    timetable[section][slot + d].subjectId = subjectId;
                    timetable[section][slot + d].teacherId = teacherId;
                    timetable[section][slot + d].roomId = roomId;
                }

                if (generate(section, slot + duration)) {
                    return true;
                }

                // Backtrack
                for (int d = 0; d < duration; d++) {
                    timetable[section][slot + d].subjectId = -1;
                    timetable[section][slot + d].teacherId = -1;
                    timetable[section][slot + d].roomId = -1;
                }
            }
        }
    }
    return false;
}
