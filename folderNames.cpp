
#include <iostream>
#include <filesystem>
#include <string>

using namespace std;
namespace fs = filesystem;

void displayFolderNames(const string& path)
{
    for (const auto& entry : fs::directory_iterator(path))
    {
        if (entry.is_directory())
        {
            cout << entry.path().filename().string() << endl;
        }
    }
}
