#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "model/Subjects.h"
#include "model/Teacher.h"
#include "model/Sections.h"
#include "model/Class.h"
#include "model/Room.h"
using namespace std;

// Global data

vector<Subject> subjects;
vector<Teacher> teachers;
vector<Section> sections;
vector<Room> rooms;

vector<string> slots = {
    "8-9", "9-10", "10-11", "11-12",
    "12-1", "1-2", "2-3", "3-4", "4-5"
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

    // Read rooms from file
    // Read rooms from file
    ifstream roomFile("input/rooms.csv");

    // skip header
    getline(roomFile, line);

    while (getline(roomFile, line)) {

        istringstream iss(line);

        string id;
        string name;

        getline(iss, id, ',');
        getline(iss, name, ',');

        rooms.push_back({
            stoi(id),
            name
        });
    }

    roomFile.close();
}

// -------------------Display timetable--------------------
void displayTimetable()
{
    cout << "\n========== ONE DAY TIMETABLE ==========\n\n";

    for (int i = 0; i < sections.size(); i++) {

        cout << "Section: " << sections[i].name << endl;

        for (int j = 0; j < slots.size(); j++) {

            int subjectId = timetable[i][j].subjectId;
            int teacherId = timetable[i][j].teacherId;
            int roomId = timetable[i][j].roomId;

            string subjectName;
            string teacherName;
            string roomName;

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

            // Find room name
            for (int k = 0; k < rooms.size(); k++) {
                if (rooms[k].id == roomId) {
                    roomName = rooms[k].name;
                    break;
                }
            }

            cout << slots[j] << " -> "
                 << subjectName << " -> "
                 << teacherName << " -> "
                 << roomName << endl;
        }

        cout << endl;
    }
}

void writeSectionCSV()
{
    ofstream file("output/section_timetable.csv");
    file << "Section,Slot,Subject,Teacher,Room\n";

    for (int i = 0; i < sections.size(); i++) {
        for (int j = 0; j < slots.size(); j++) {
            int sid = timetable[i][j].subjectId;
            file << sections[i].name << "," << slots[j] << ",";

            if (sid == -2) {
                file << "BREAK,,";
            }
            else if (sid == -1) {
                file << "FREE,,";
            }
            else {
                for (int k = 0; k < subjects.size(); k++)
                    if (subjects[k].id == sid)
                        file << subjects[k].name << ",";

                for (int k = 0; k < teachers.size(); k++)
                    if (teachers[k].id == timetable[i][j].teacherId)
                        file << teachers[k].name << ",";

                for (int k = 0; k < rooms.size(); k++)
                    if (rooms[k].id == timetable[i][j].roomId)
                        file << rooms[k].name;
            }

            file << "\n";
        }
    }

    file.close();
}

void writeTeacherCSV()
{
    ofstream file("output/teacher_timetable.csv");
    file << "Teacher,Slot,Subject,Section,Room\n";

    for (int i = 0; i < teachers.size(); i++) {
        for (int j = 0; j < slots.size(); j++) {
            file << teachers[i].name << "," << slots[j] << ",";

            bool found = false;

            for (int k = 0; k < sections.size(); k++) {
                if (timetable[k][j].teacherId == teachers[i].id) {
                    int sid = timetable[k][j].subjectId;

                    for (int m = 0; m < subjects.size(); m++)
                        if (subjects[m].id == sid)
                            file << subjects[m].name;

                    file << "," << sections[k].name << ",";

                    for (int m = 0; m < rooms.size(); m++)
                        if (rooms[m].id == timetable[k][j].roomId)
                            file << rooms[m].name;

                    found = true;
                    break;
                }
            }

            if (!found)
                file << "FREE,,";

            file << "\n";
        }
    }

    file.close();
}


void writeRoomCSV()
{
    ofstream file("output/room_timetable.csv");
    file << "Room,Slot,Subject,Teacher,Section\n";

    for (int i = 0; i < rooms.size(); i++) {
        for (int j = 0; j < slots.size(); j++) {
            file << rooms[i].name << "," << slots[j] << ",";

            bool found = false;

            for (int k = 0; k < sections.size(); k++) {
                if (timetable[k][j].roomId == rooms[i].id) {
                    int sid = timetable[k][j].subjectId;

                    for (int m = 0; m < subjects.size(); m++)
                        if (subjects[m].id == sid)
                            file << subjects[m].name;

                    file << ",";

                    for (int m = 0; m < teachers.size(); m++)
                        if (teachers[m].id == timetable[k][j].teacherId)
                            file << teachers[m].name;

                    file << "," << sections[k].name;
                    found = true;
                    break;
                }
            }

            if (!found)
                file << "FREE,,";

            file << "\n";
        }
    }

    file.close();
}




void writeTimetable()
{
    ofstream file("output/timetable.csv");

    file << "Section,Time,Subject,Teacher,Room\n";

    for (int i = 0; i < sections.size(); i++) {

        for (int j = 0; j < slots.size(); j++) {

            int subjectId = timetable[i][j].subjectId;
            int teacherId = timetable[i][j].teacherId;
            int roomId = timetable[i][j].roomId;

            string subjectName = "Empty";
            string teacherName = "Empty";
            string roomName = "Empty";

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

            // Find room name
            for (int k = 0; k < rooms.size(); k++) {
                if (rooms[k].id == roomId) {
                    roomName = rooms[k].name;
                    break;
                }
            }

            // Save timetable data into CSV
            file << sections[i].name << ","
                 << slots[j] << ","
                 << subjectName << ","
                 << teacherName << ","
                 << roomName << "\n";
        }
    }

    file.close();

    cout << "Timetable saved successfully!\n";
}

//________________________________BREAK______________________

void takeBreaksFromFile(string folder)
{
    ifstream file(folder + "/breaks.csv");

    if (!file.is_open()) {
        cout << "Error opening breaks.csv\n";
        return;
    }

    string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        stringstream ss(line);

        string sectionId, slot;
        getline(ss, sectionId, ',');
        getline(ss, slot, ',');

        int section = stoi(sectionId) - 1;
        int s = stoi(slot);

        if (section >= 0 && section < sections.size() &&
            s >= 0 && s < slots.size()) {

            timetable[section][s].subjectId = -2;
            timetable[section][s].teacherId = -1;
            timetable[section][s].roomId = -1;
        }
        else {
            cout << "Invalid break: " << line << "\n";
        }
    }

    
    file.close();
}

void displayFolderNames(const string& path);


int main()
{
   cout << "Project folders:\n";
    displayFolderNames(".");

    takeInputFromFiles();
    
   timetable.resize(sections.size());

    for (int i = 0; i < sections.size(); i++) {
        for (int j = 0; j < slots.size(); j++) {
            timetable[i].push_back({-1, -1, -1});
        }
    }

    //Set Brakes
    takeBreaksFromFile("input");

    if (generate(0, 0)) {
        cout << "Timetable generated successfully!\n";
        displayTimetable();
        writeTimetable();

        writeSectionCSV();
        writeTeacherCSV();
        writeRoomCSV();
    }
    else {
        cout << "No valid timetable possible!\n";
    }

    return 0;
}
