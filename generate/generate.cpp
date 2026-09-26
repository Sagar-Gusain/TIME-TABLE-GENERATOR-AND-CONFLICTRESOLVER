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
        }
    }

    return false;
}
