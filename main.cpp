#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <algorithm>
#include "model/Subjects.h"
#include "model/Teacher.h"
#include "model/Sections.h"
#include "model/Class.h"
#include "model/Room.h"

using namespace std;
namespace fs = std::filesystem;

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

string inputFolder = "";
string outputFolder = "output";

// Generation code
#include "generate/generate.cpp"

// Helper to trim whitespace and carriage returns
static string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// Find input directory whether run from project root or workspace root
string findInputDirectory() {
    if (fs::exists("input") && fs::is_directory("input")) {
        return "input";
    }
    if (fs::exists("TIME-TABLE-GENERATOR-AND-CONFLICTRESOLVER/input") && 
        fs::is_directory("TIME-TABLE-GENERATOR-AND-CONFLICTRESOLVER/input")) {
        return "TIME-TABLE-GENERATOR-AND-CONFLICTRESOLVER/input";
    }
    return "";
}

// ---------------Take input from files------------------

void takeInputFromFiles(string folder) {
    // Read subjects from file
    ifstream subjectFile(folder + "/subjects.csv");
    if (!subjectFile.is_open()) {
        cout << "Error opening " << folder << "/subjects.csv\n";
        return;
    }

    string line;
    // skip header
    getline(subjectFile, line);

    while (getline(subjectFile, line)) {
        line = trim(line);
        if (line.empty()) continue;

        istringstream iss(line);
        string id, name, duration;
        getline(iss, id, ',');
        getline(iss, name, ',');
        getline(iss, duration, ',');

        subjects.push_back({stoi(trim(id)), trim(name), stoi(trim(duration))});
    }
    subjectFile.close();

    // Read teachers from file
    ifstream teacherFile(folder + "/teachers.csv");
    if (!teacherFile.is_open()) {
        cout << "Error opening " << folder << "/teachers.csv\n";
        return;
    }

    getline(teacherFile, line);

    while (getline(teacherFile, line)) {
        line = trim(line);
        if (line.empty()) continue;

        istringstream iss(line);
        string id, name, subjectId;
        getline(iss, id, ',');
        getline(iss, name, ',');
        getline(iss, subjectId, ',');

        teachers.push_back({
            stoi(trim(id)),
            trim(name),
            stoi(trim(subjectId))
        });
    }
    teacherFile.close();

    // Read sections from file
    ifstream sectionFile(folder + "/sections.csv");
    if (!sectionFile.is_open()) {
        cout << "Error opening " << folder << "/sections.csv\n";
        return;
    }

    getline(sectionFile, line);

    while (getline(sectionFile, line)) {
        line = trim(line);
        if (line.empty()) continue;

        istringstream iss(line);
        string id, name;
        getline(iss, id, ',');
        getline(iss, name, ',');

        sections.push_back({
            stoi(trim(id)),
            trim(name)
        });
    }
    sectionFile.close();

    // Read rooms from file
    ifstream roomFile(folder + "/rooms.csv");
    if (!roomFile.is_open()) {
        cout << "Error opening " << folder << "/rooms.csv\n";
        return;
    }

    getline(roomFile, line);

    while (getline(roomFile, line)) {
        line = trim(line);
        if (line.empty()) continue;

        istringstream iss(line);
        string id, name;
        getline(iss, id, ',');
        getline(iss, name, ',');

        rooms.push_back({
            stoi(trim(id)),
            trim(name)
        });
    }
    roomFile.close();
}

// -------------------Display timetable--------------------
void displayTimetable()
{
    cout << "\n========== ONE DAY TIMETABLE ==========\n\n";

    for (size_t i = 0; i < sections.size(); i++) {

        cout << "Section: " << sections[i].name << endl;

        for (size_t j = 0; j < slots.size(); j++) {

            int subjectId = timetable[i][j].subjectId;
            int teacherId = timetable[i][j].teacherId;
            int roomId = timetable[i][j].roomId;

            string subjectName = "FREE";
            string teacherName = "-";
            string roomName = "-";

            if (subjectId == -2) {
                subjectName = "BREAK";
            }
            else if (subjectId != -1) {
                // Find subject name
                for (size_t k = 0; k < subjects.size(); k++) {
                    if (subjects[k].id == subjectId) {
                        subjectName = subjects[k].name;
                        break;
                    }
                }

                // Find teacher name
                for (size_t k = 0; k < teachers.size(); k++) {
                    if (teachers[k].id == teacherId) {
                        teacherName = teachers[k].name;
                        break;
                    }
                }

                // Find room name
                for (size_t k = 0; k < rooms.size(); k++) {
                    if (rooms[k].id == roomId) {
                        roomName = rooms[k].name;
                        break;
                    }
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
    ofstream file(outputFolder + "/section_timetable.csv");
    file << "Section,Slot,Subject,Teacher,Room\n";

    for (size_t i = 0; i < sections.size(); i++) {
        for (size_t j = 0; j < slots.size(); j++) {
            int sid = timetable[i][j].subjectId;
            file << sections[i].name << "," << slots[j] << ",";

            if (sid == -2) {
                file << "BREAK,,";
            }
            else if (sid == -1) {
                file << "FREE,,";
            }
            else {
                for (size_t k = 0; k < subjects.size(); k++)
                    if (subjects[k].id == sid)
                        file << subjects[k].name << ",";

                for (size_t k = 0; k < teachers.size(); k++)
                    if (teachers[k].id == timetable[i][j].teacherId)
                        file << teachers[k].name << ",";

                for (size_t k = 0; k < rooms.size(); k++)
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
    ofstream file(outputFolder + "/teacher_timetable.csv");
    file << "Teacher,Slot,Subject,Section,Room\n";

    for (size_t i = 0; i < teachers.size(); i++) {
        for (size_t j = 0; j < slots.size(); j++) {
            file << teachers[i].name << "," << slots[j] << ",";

            bool found = false;

            for (size_t k = 0; k < sections.size(); k++) {
                if (timetable[k][j].teacherId == teachers[i].id) {
                    int sid = timetable[k][j].subjectId;

                    for (size_t m = 0; m < subjects.size(); m++)
                        if (subjects[m].id == sid)
                            file << subjects[m].name;

                    file << "," << sections[k].name << ",";

                    for (size_t m = 0; m < rooms.size(); m++)
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
    ofstream file(outputFolder + "/room_timetable.csv");
    file << "Room,Slot,Subject,Teacher,Section\n";

    for (size_t i = 0; i < rooms.size(); i++) {
        for (size_t j = 0; j < slots.size(); j++) {
            file << rooms[i].name << "," << slots[j] << ",";

            bool found = false;

            for (size_t k = 0; k < sections.size(); k++) {
                if (timetable[k][j].roomId == rooms[i].id) {
                    int sid = timetable[k][j].subjectId;

                    for (size_t m = 0; m < subjects.size(); m++)
                        if (subjects[m].id == sid)
                            file << subjects[m].name;

                    file << ",";

                    for (size_t m = 0; m < teachers.size(); m++)
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
    ofstream file(outputFolder + "/timetable.csv");

    file << "Section,Time,Subject,Teacher,Room\n";

    for (size_t i = 0; i < sections.size(); i++) {

        for (size_t j = 0; j < slots.size(); j++) {

            int subjectId = timetable[i][j].subjectId;
            int teacherId = timetable[i][j].teacherId;
            int roomId = timetable[i][j].roomId;

            string subjectName = "FREE";
            string teacherName = "-";
            string roomName = "-";

            if (subjectId == -2) {
                subjectName = "BREAK";
            }
            else if (subjectId != -1) {
                // Find subject name
                for (size_t k = 0; k < subjects.size(); k++) {
                    if (subjects[k].id == subjectId) {
                        subjectName = subjects[k].name;
                        break;
                    }
                }

                // Find teacher name
                for (size_t k = 0; k < teachers.size(); k++) {
                    if (teachers[k].id == teacherId) {
                        teacherName = teachers[k].name;
                        break;
                    }
                }

                // Find room name
                for (size_t k = 0; k < rooms.size(); k++) {
                    if (rooms[k].id == roomId) {
                        roomName = rooms[k].name;
                        break;
                    }
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
        cout << "Error opening " << folder << "/breaks.csv\n";
        return;
    }

    string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        line = trim(line);
        if (line.empty()) continue;

        stringstream ss(line);

        string sectionId, slot;
        getline(ss, sectionId, ',');
        getline(ss, slot, ',');

        int section = stoi(trim(sectionId)) - 1;
        int s = stoi(trim(slot));

        if (section >= 0 && section < static_cast<int>(sections.size()) &&
            s >= 0 && s < static_cast<int>(slots.size())) {

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


int main()
{
    cout << "Program started!" << endl;

    string inputBase = findInputDirectory();
    if (inputBase.empty())
    {
        cout << "No 'input' directory found! Please make sure the 'input' folder exists.\n";
        return 0;
    }

    if (inputBase == "TIME-TABLE-GENERATOR-AND-CONFLICTRESOLVER/input") {
        outputFolder = "TIME-TABLE-GENERATOR-AND-CONFLICTRESOLVER/output";
    } else {
        outputFolder = "output";
    }
    fs::create_directories(outputFolder);

    vector<string> folders;

    for (const auto& entry : fs::directory_iterator(inputBase))
    {
        if (entry.is_directory())
        {
            folders.push_back(entry.path().string());
        }
    }

    if (folders.empty())
    {
        if (fs::exists(inputBase + "/subjects.csv")) {
            folders.push_back(inputBase);
        } else {
            cout << "No input folders or CSV files found inside " << inputBase << "!\n";
            return 0;
        }
    }

    sort(folders.begin(), folders.end());

    cout << "Choose an input folder:\n";

    for (size_t i = 0; i < folders.size(); i++)
    {
        cout << i + 1 << ". "
             << fs::path(folders[i]).filename().string()
             << "\n";
    }

    int choice;
    cout << "Enter choice: ";
    if (!(cin >> choice))
    {
        cout << "Invalid input!\n";
        return 0;
    }

    if (choice < 1 || choice > static_cast<int>(folders.size()))
    {
        cout << "Invalid choice!\n";
        return 0;
    }

    inputFolder = folders[choice - 1];

    cout << "Selected folder: " << inputFolder << "\n";

    takeInputFromFiles(inputFolder);

    timetable.resize(sections.size());

    for (size_t i = 0; i < sections.size(); i++) {
        for (size_t j = 0; j < slots.size(); j++) {
            timetable[i].push_back({-1, -1, -1});
        }
    }

    takeBreaksFromFile(inputFolder);

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
