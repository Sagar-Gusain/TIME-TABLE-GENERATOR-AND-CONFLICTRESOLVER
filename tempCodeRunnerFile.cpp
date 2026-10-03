oid displayTimetable(){
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
