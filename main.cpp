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
    "08:00-08:30", "08:30-09:00", "09:00-09:30", "09:30-10:00",
    "10:00-10:30", "10:30-11:00", "11:00-11:30", "11:30-12:00"
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

            // Track continuous block span to group back together for output presentation
            int sequenceStart = currentSlotIndex;
            while (currentSlotIndex < (int)slots.size() && 
                   timetable[i][currentSlotIndex].subjectId == currentSubject && 
                   timetable[i][currentSlotIndex].roomId == currentRoom) {
                currentSlotIndex++;
            }
            int sequenceEnd = currentSlotIndex - 1;

            // Standard string slicing to isolate start boundaries and end boundaries
            string openTime = slots[sequenceStart].substr(0, 5);
            string closeTime = slots[sequenceEnd].substr(6, 5);
            string formattedRange = openTime + "-" + closeTime;

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

    // Call backtracking execution loop
    if (generate(0, 0)) {
        cout << "Generation successful." << endl;
        writeTimetable();
    } else {
        cout << "Processing error:." << endl;
    }
    
    return 0;
}
