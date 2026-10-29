
#include <iostream>
#include <filesystem>
#include <string>

using namespace std;
namespace fs = filesystem;

void displayFolderNames(const string& path)
{
    try {
        for (const auto& entry : fs::directory_iterator(path)) {
            if (entry.is_directory()) {
                cout << entry.path().filename().string() << endl;
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        cout << "Error: " << e.what() << endl;
    }
}

int main()
{
    cout << "Folders in the current directory:\n";
    displayFolderNames(".");
    return 0;
}

