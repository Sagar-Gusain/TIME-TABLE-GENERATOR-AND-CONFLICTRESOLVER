#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "model/Subjects.h"
#include "model/Teacher.h"
#include "model/Sections.h"
#include "model/Class.h"

using namespace std;

// Global data

vector<Subject> subjects;
vector<Teacher> teachers;
vector<Section> sections;
vector<Room> rooms;

vector<string> slots = {
    "8-9", "9-10", "10-11", "11-12"
};

vector<vector<Class>> timetable;


// Generation code
#include "generate/generate.cpp"

// ---------------Take input from files------------------


void takeInputFromFiles(){
    // Read subjects from file
    ifstream subjectFile("input/subjects.csv");

    string line;

    // skip header
    getline(subjectFile, line);

    while (getline(subjectFile, line)) {

        istringstream iss(line);

        string id;
        string name;
        string duration;
        getline(iss, id, ',');
        getline(iss, name, ',');
        getline(iss, duration, ',');

        subjects.push_back({stoi(id), name, stoi(duration)});
    }
    subjectFile.close();

    // Read teachers from file
    ifstream teacherFile("input/teachers.csv");
    // skip header
    getline(teacherFile, line);

    while (getline(teacherFile, line)) {
        istringstream iss(line);

        string id;
        string name;
        string subjectId;

        getline(iss, id, ',');
        getline(iss, name, ',');
        getline(iss, subjectId, ',');

        teachers.push_back({
            stoi(id),
            name,
            stoi(subjectId)
        });
    }

    teacherFile.close();
    // Read sections from file
    ifstream sectionFile("input/sections.csv");

    // skip header
    getline(sectionFile, line);

    while (getline(sectionFile, line)) {
        istringstream iss(line);

        string id;
        string name;

        getline(iss, id, ',');
        getline(iss, name, ',');

        sections.push_back({
            stoi(id),
            name
        });
    }

    sectionFile.close();
}

// -------------------Display timetable--------------------
void displayTimetable(){
    cout << "\n========== ONE DAY TIMETABLE ==========\n\n";

    for (int i = 0; i < sections.size(); i++) {

        cout << "Section: " << sections[i].name << endl;

        for (int j = 0; j < slots.size(); j++) {

            int subjectId = timetable[i][j].subjectId;
            int teacherId = timetable[i][j].teacherId;

            string subjectName ;
            string teacherName ;

            // Find subject name
            for (int k = 0; k < subjects.size(); k++) {

                if (subjects[k].id == subjectId) {
                    subjectName = subjects[k].name;
                    break;
                }
            }

            // Find teacher name
            for (int k = 0; k < teachers.size(); k++) {

                if (teachers[k].id == teacherId) {
                    teacherName = teachers[k].name;
                    break;
                }
            }
            cout << slots[j] << " -> "
                 << subjectName << " -> "
                 << teacherName << endl;
        }

        cout << endl;
    }
}

int main()
{
    takeInputFromFiles();

   timetable.resize(sections.size());

    for (int i = 0; i < sections.size(); i++) {
        for (int j = 0; j < slots.size(); j++) {
            timetable[i].push_back({-1, -1});
        }
    }

    if (generate(0, 0)) {
        cout << "Timetable generated successfully!\n";
        displayTimetable();
    }
    else {
        cout << "No valid timetable possible!\n";
    }

    return 0;
}