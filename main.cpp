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

// Global vectors holding data loaded from CSVs
vector<Subject> subjects;
vector<Teacher> teachers;
vector<Section> sections;
vector<Room> rooms;

// 30 minute timeline divisions to handle fractional lecture durations
vector<string> slots = {
    "8-9", "9-10", "10-11", "11-12",
    "12-1", "1-2", "2-3", "3-4", "4-5"
};

// 2D grid matrix: rows represent sections, columns represent timeline intervals
vector<vector<Class>> timetable;

#include "generate/generate.cpp"

// Read configuration entries from file system
void takeInputFromFiles(){
    ifstream subjectFile("input/subjects.csv");
    string line;
    
    // bypass data headers
    getline(subjectFile, line);
    while (getline(subjectFile, line)) {
        istringstream iss(line);
        string id, name, duration;
        
        getline(iss, id, ',');
        getline(iss, name, ',');
        getline(iss, duration, ',');

        double hourValue = stod(duration);
        // Calculate raw block footprint based on half-hour step increments
        int equivalentSlots = (int)(hourValue * 2); 

        subjects.push_back({stoi(id), name, hourValue, equivalentSlots});
    }
    subjectFile.close();

    ifstream teacherFile("input/teachers.csv");
    getline(teacherFile, line);
    while (getline(teacherFile, line)) {
        istringstream iss(line);
        string id, name, subjectId;
        
        getline(iss, id, ',');
        getline(iss, name, ',');
        getline(iss, subjectId, ',');
        
        teachers.push_back({stoi(id), name, stoi(subjectId)});
    }
    teacherFile.close();

    ifstream sectionFile("input/sections.csv");
    getline(sectionFile, line);
    while (getline(sectionFile, line)) {
        istringstream iss(line);
        string id, name;
        
        getline(iss, id, ',');
        getline(iss, name, ',');
        
        sections.push_back({stoi(id), name});
    }
    sectionFile.close();

    ifstream roomFile("input/rooms.csv");
    getline(roomFile, line);
    while (getline(roomFile, line)) {
        istringstream iss(line);
        string id, name;
        
        getline(iss, id, ',');
        getline(iss, name, ',');
        
        rooms.push_back({stoi(id), name});
    }
    roomFile.close();
}

// Write the compiled matrix grid back into an export file
void writeTimetable()
{
    ofstream outputFile("output/timetable.csv");
    outputFile << "Section,Time,Subject,Teacher,Room\n";

    for (size_t i = 0; i < sections.size(); i++) {
        int currentSlotIndex = 0;
        
        while (currentSlotIndex < (int)slots.size()) {
            int currentSubject = timetable[i][currentSlotIndex].subjectId;
            int currentTeacher = timetable[i][currentSlotIndex].teacherId;
            int currentRoom = timetable[i][currentSlotIndex].roomId;

            // Handle vacant intervals safely
            if (currentSubject == -1) {
                outputFile << sections[i].name << "," 
                           << slots[currentSlotIndex] 
                           << ",Empty,Empty,Empty\n";
                currentSlotIndex++;
                continue;
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

            string matchedSubject = "Empty";
            string matchedTeacher = "Empty";
            string matchedRoom = "Empty";

            // Loop lookups matching metadata attributes
            for (size_t tempIdx = 0; tempIdx < subjects.size(); tempIdx++) {
                if (subjects[tempIdx].id == currentSubject) { 
                    matchedSubject = subjects[tempIdx].name; 
                    break; 
                }
            }
            for (size_t tempIdx = 0; tempIdx < teachers.size(); tempIdx++) {
                if (teachers[tempIdx].id == currentTeacher) { 
                    matchedTeacher = teachers[tempIdx].name; 
                    break; 
                }
            }
            for (size_t tempIdx = 0; tempIdx < rooms.size(); tempIdx++) {
                if (rooms[tempIdx].id == currentRoom) { 
                    matchedRoom = rooms[tempIdx].name; 
                    break; 
                }
            }

            outputFile << sections[i].name << "," 
                       << formattedRange << "," 
                       << matchedSubject << "," 
                       << matchedTeacher << "," 
                       << matchedRoom << "\n";
        }
    }
    outputFile.close();
    cout << "Process finished: CSV schedule updated." << endl;
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


int main()
{
    // Populate data structures
    takeInputFromFiles();
    
    // Allocate space parameters matching sections size
    timetable.resize(sections.size());

    // Initialize allocations with blank states
    for (size_t i = 0; i < sections.size(); i++) {
        for (size_t j = 0; j < slots.size(); j++) {
            timetable[i].push_back({-1, -1, -1});
        }
    }

    //Set Brakes
    takeBreaksFromFile("input");

    if (generate(0, 0)) {
        cout << "Generation successful." << endl;
        writeTimetable();
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
