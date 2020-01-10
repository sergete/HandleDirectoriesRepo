/**
 * Copy all the files from one directory to another avoiding to copy duplicates files
 * @author Sergio Sánchez-Uran López
 * */

#include <iostream>
#include <fstream>
#include <algorithm>
#include <experimental/filesystem>

const std::string SLASH = "/";

namespace fs = std::experimental::filesystem;
using namespace std;

/**
 * Compare files and return true if both are equals, false if not
 * */
bool compare_files(const std::string& filename1, const std::string& filename2)
{
    std::ifstream file1(filename1, std::ifstream::ate | std::ifstream::binary); //open file at the end
    std::ifstream file2(filename2, std::ifstream::ate | std::ifstream::binary); //open file at the end

    //different file size then are not equals
    if (file1.tellg() != file2.tellg()) {
        return false;
    }

    file1.seekg(0); //rewind to file start
    file2.seekg(0); //rewind to file start

    //iterators
    std::istreambuf_iterator<char> begin1(file1);
    std::istreambuf_iterator<char> begin2(file2);

    //Compare if two files has same info from beginning to end.
    return std::equal(begin1,std::istreambuf_iterator<char>(),begin2); //Second argument is end-of-range iterator
}

/**
 * Move file from one directory to another
 * */
void transferFile(std::string *file, std::string *pathToLocate){
    const int idx = file->find_last_of("\\/");
    std::string name = "";
    if (std::string::npos != idx)
    {
        name = file->substr(idx + 1);
        name = *pathToLocate + SLASH + name;
    }
    std::experimental::filesystem::copy(file->c_str(), name.c_str());
}

int main() {
    //Read all names of directory to load all replays to extract data
    std::cout << "Introduce directory paths to transfer files" << std::endl;
    std::cout << "From: ";
    std::string path;
    std::cin >> path;
    std::cout << endl;
    std::cout << "To: ";
    std::string pathToLocate;
    std::cin >> pathToLocate;
    std::cout << endl;

    int contadorDirectorioFrom = 0;
    int contadorDirectorioTo = 0;

    for (const auto & entryFrom : fs::directory_iterator(path)) {
        //std::cout << entry.path() << std::endl;
        std::string file = entryFrom.path().generic_string();
        contadorDirectorioFrom++;
        bool fileExists = false;
        for (const auto & entryDestiny : fs::directory_iterator(pathToLocate)) {
            std::string file2 = entryDestiny.path().generic_string();
            fileExists = compare_files(file, file2);
            if(fileExists){
                break;
            }
        }

        if(!fileExists){
            transferFile(&file, &pathToLocate);
            contadorDirectorioTo++;
        }
    }
    std::cout << endl;
    std::cout << "Files founded in " << path << " : " << contadorDirectorioFrom << endl;
    std::cout << "Files copied to " << pathToLocate << " : " << contadorDirectorioTo << endl;
    std::cout << "Duplicates files : " << (contadorDirectorioFrom - contadorDirectorioTo) << endl;

    //clear input in memory first
    std::cin.ignore();
    std::cout << "press enter to exit\n";
    std::cin.ignore();
    return 0;
}
