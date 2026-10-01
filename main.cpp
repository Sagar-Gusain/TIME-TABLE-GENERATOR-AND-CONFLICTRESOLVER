#include <iostream>
#include <vector>
#include <string>

#include "model/Subjects.h"
#include "model/Teacher.h"
#include "model/Sections.h"
#include "model/Class.h"
#include "model/Room.h"

using namespace std;

// ---------------- GLOBAL DATA ----------------

vector<Subject> subjects;
vector<Teacher> teachers;
vector<Section> sections;

vector<string> slots = {
    "8-9", "9-10", "10-11", "11-12",
    "12-1", "1-2", "2-3", "3-4"
};

// timetable[section][slot]
vector<vector<Class>> timetable;


int main() {

    // ---------------- TEACHERS ----------------

    teachers.push_back({1, "Dr. Sharma", 1});
    teachers.push_back({2, "Dr. Verma", 2});
    teachers.push_back({3, "Dr. Singh", 3});
    teachers.push_back({4, "Dr. Gupta", 4});


    // ---------------- SUBJECTS ----------------

    subjects.push_back({1, "Data Structures", 4});
    subjects.push_back({2, "OOP", 4});
    subjects.push_back({3, "Mathematics", 4});
    subjects.push_back({4, "Computer Networks", 3});


    // ---------------- SECTIONS ----------------

    sections.push_back({1, "CSE-G"});
    sections.push_back({2, "CSE-C"});


    // ---------------- INITIALIZE TIMETABLE ----------------

    timetable.resize(sections.size());

    for (int i = 0; i < sections.size(); i++) {
        timetable[i].resize(slots.size());
    }


    // ---------------- DISPLAY INPUT DATA ----------------

    cout << "===== TIMETABLE GENERATOR =====" << endl;

    cout << "\nTeachers:" << endl;

    for (Teacher teacher : teachers) {
        cout << teacher.id << ". "
             << teacher.name << endl;
    }

    cout << "\nSubjects:" << endl;

    for (Subject subject : subjects) {
        cout << subject.id << ". "
             << subject.name
             << " (" << subject.lecturesPerWeek
             << " lectures/week)" << endl;
    }

    cout << "\nSections:" << endl;

    for (Section section : sections) {
        cout << section.id << ". "
             << section.name << endl;
    }

    cout << "\nTime Slots:" << endl;

    for (string slot : slots) {
        cout << slot << endl;
    }

    cout << "\nTimetable size: "
         << timetable.size()
         << " sections x "
         << slots.size()
         << " time slots"
         << endl;

    return 0;
}
