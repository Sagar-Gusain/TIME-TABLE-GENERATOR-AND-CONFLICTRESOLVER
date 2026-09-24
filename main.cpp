#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "model/Room.h"
#include "model/Subjects.h"
#include "model/Teacher.h"

using namespace std;

class Generate;  

class TimeTable {
private:
    vector<Subject> subjects;
    vector<Teacher> teachers;
    vector<Room> rooms;

    vector<string> slots = {
        "8-9", "9-10", "10-11", "11-12",
        "12-1", "1-2", "2-3", "3-4"
    };

};


int main() {
   
}