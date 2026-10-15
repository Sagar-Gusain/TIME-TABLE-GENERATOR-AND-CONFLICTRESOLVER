#include <string>
using namespace std;

struct Subject {
    int id;
    string name;
    float duration; // Duration in hours and double so that 2.5,1.5 hours can be supported
    int slotsNeeded;//it will hold the converted integer count of 30 min intervals
};
#endif
