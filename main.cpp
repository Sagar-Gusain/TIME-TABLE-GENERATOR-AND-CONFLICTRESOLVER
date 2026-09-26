#include <iostream>
#include <vector>
#include <string>

#include "model/Subjects.h"
#include "model/Teacher.h"
#include "model/Sections.h"
#include "model/Class.h"
#include "model/Room.h"

#include "generate/generate.cpp"

// ---------------- GLOBAL DATA ----------------

vector<Subject> subjects;
vector<Teacher> teachers;
vector<Section> sections;

vector<string> slots = {
    "8-9", "9-10", "10-11", "11-12",
    "12-1", "1-2", "2-3", "3-4"
};

//timetable taking row=sections and col=slots
// timetable[section][slot]
vector<vector<Class>> timetable;

int main() {
   
}