#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

int main() {
    std::ifstream inputFile("../assets.csv");
    std::ofstream outputFile("../InsertStudents.txt");

    if (!inputFile.is_open() || !outputFile.is_open()) {
        std::cerr << "Failed to open the input files." << std::endl;
        return 1;
    }

    std::string line;
    std::vector<std::string> subjects;

    // Read header and save subject names
    if (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        std::string subject;
        while (std::getline(ss, subject, ';')) { // Use semicolon as delimiter
            if (subject != "Группа" && subject != "ФИО") {
                subjects.push_back(subject);
            }
        }
    }

    // Read data
    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        std::string group, name;

        std::getline(ss, group, ';'); // Read group
        std::getline(ss, name, ';');  // Read name

        // Process grades
        for (size_t i = 0; i < subjects.size(); ++i) {
            std::string grade;
            if (std::getline(ss, grade, ';')) { // Use semicolon as delimiter
                if (!grade.empty()) { // Skip empty cells
                    outputFile << "INSERT INTO students VALUES ("
                               << group << ", '" << name << "', '"
                               << subjects[i] << "', '" << grade << "');" << std::endl;
                }
            }
        }
    }

    inputFile.close();
    outputFile.close();

    std::cout << "SQL statements have been successfully written to InsertStudents.txt." << std::endl;
    return 0;
}