#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <limits>

using namespace std;

class PersonType
{
    public:
    PersonType() : Name("") {}
    void setName(string name) { Name = name; }
    string getName() const { return Name; }
    void print() const { cout << Name << " "; }
    private:
    string Name;
};

class CourseType
{
    public:
    CourseType() : courseName(""), courseNo(""), courseCredits(0), courseGrade('x') {}
    CourseType(string name, string no, int credit, char grade);
    
    void setCourseInfo(string name, string no, int credit, char grade) {
        if (credit < 0) throw invalid_argument("Credit hours cannot be negative");
        if (grade != 'A' && grade != 'B' && grade != 'C' && grade != 'D' && grade != 'F')
            throw invalid_argument("Invalid grade");
            
        courseName = name;
        courseNo = no;
        courseCredits = credit;
        courseGrade = grade;
    }

    string getName() const { return courseName; }
    int getCredits() const { return courseCredits; }
    string getCourseNo() const { return courseNo; }
    char getCourseGrade() const { return courseGrade; }

    private:
    string courseName;
    string courseNo;
    int courseCredits;
    char courseGrade;
};

CourseType::CourseType(string name,string no, int credit,char grade) 
{
    courseName=name;
    courseNo=no;
    courseCredits=credit;
    courseGrade=grade;
} 



class StudentType
{
    public:
    StudentType(int size);
    ~StudentType();
    void print();
    void print(string a);
    void readInput(string fileName);
    void getHoursEnrolled();
    void getGPA();
    void billingAmmount();
    void printSortingResults(int studentIndex, bool toFile = false, ofstream* outputFile = nullptr);
    void askAndShowSortingResults();
    void searchStudent();
    void printStudentDetails(int studentIndex, bool toFile = false, ofstream* outputFile = nullptr);

    // New sorting functions
    void sortCoursesByGrade(int studentIndex);
    void sortCoursesByCredit(int studentIndex);
    void sortCoursesByName(int studentIndex);
    void printSortedCourses(int studentIndex);

    void writeSearchResultsToFile(const vector<int>& matches, int selectedIndex);

    private:
    string* Id;
    char* isTuitonPaid;
    int* numberOfCourses;
    string* CourseNames;
    string* CourseGrades;
    double* GPA;
    int* totalHours;
    int* bill;
    PersonType* person;
    CourseType** course;
    int Ucret;
    int Size;
    bool showSortingResults;
};

StudentType::StudentType(int size) {
    Id = new string[size];
    isTuitonPaid = new char[size];
    numberOfCourses = new int[size];
    CourseNames = new string[size];
    CourseGrades = new string[size];
    GPA = new double[size];
    totalHours = new int[size];
    bill = new int[size];
    person = new PersonType[size];
    course = new CourseType*[size];
    Size = size;
    showSortingResults = false;

    for (int i = 0; i < size; i++) {
        person[i].setName("");
        Id[i] = "";
        isTuitonPaid[i] = '-';
        numberOfCourses[i] = 0;
        CourseNames[i] = "";
        CourseGrades[i] = "";
        GPA[i] = 0;
        totalHours[i] = 0;
        bill[i] = 0;
    }
}

StudentType::~StudentType(){
    delete[] Id;
    delete[] isTuitonPaid;
    delete[] numberOfCourses;
    delete[] CourseNames;
    delete[] CourseGrades;
    delete[] totalHours;
    delete[] GPA;
    delete[] bill;
    delete[] person;
    for (int i = 0; i < Size; i++) {
        delete[] course[i];
    }
    delete[] course;
}

void StudentType::readInput(string filename) {
    ifstream input(filename);
    if (!input.is_open()) {
        throw runtime_error("Could not open file: " + filename);
    }

    string text;
    int ogrenciSayisi, ucret;
    int index = 0;
    
    if (!getline(input, text)) {
        throw runtime_error("File is empty");
    }
    
    stringstream ss(text);
    if (!(ss >> ogrenciSayisi >> ucret)) {
        throw runtime_error("Invalid format in first line");
    }
    
    if (ogrenciSayisi <= 0 || ucret < 0) {
        throw runtime_error("Invalid student count or fee amount");
    }
    
    Ucret = ucret;

    while (getline(input, text) && index < Size) {
        int dersSayisi;
        string surname, temp;
                                                                       
        stringstream line(text);
        if (!(getline(line, temp, ' ') && getline(line, surname, ' '))) {
            throw runtime_error("Invalid student name format");
        }
        
        temp = temp + " " + surname;
        person[index].setName(temp);
        
        if (!(getline(line, temp, ' '))) {
            throw runtime_error("Invalid ID format");
        }
        Id[index] = temp;
        
        if (!(getline(line, temp, ' '))) {
            throw runtime_error("Invalid tuition status format");
        }
        if (temp[0] != 'Y' && temp[0] != 'N') {
            throw runtime_error("Invalid tuition status (must be Y or N)");
        }
        isTuitonPaid[index] = temp[0];
        
        if (!(getline(line, temp))) {
            throw runtime_error("Invalid course count format");
        }
        try {
            dersSayisi = stoi(temp);
            if (dersSayisi < 0) {
                throw runtime_error("Course count cannot be negative");
            }
        } catch (const exception& e) {
            throw runtime_error("Invalid course count");
        }

        numberOfCourses[index] = dersSayisi;
        course[index] = new CourseType[dersSayisi];
        
        for (int i = 0; i < dersSayisi; i++) {
            if (!getline(input, text)) {
                throw runtime_error("Unexpected end of file while reading courses");
            }
            
            string cId, cHour, cGrade;
            stringstream line(text);
            
            if (!(getline(line, temp, ' ') && 
                  getline(line, cId, ' ') && 
                  getline(line, cHour, ' ') && 
                  getline(line, cGrade, ' '))) {
                throw runtime_error("Invalid course format");
            }
            
            CourseNames[index] += temp + " ";
            CourseGrades[index] += cGrade;
            
            try {
                course[index][i].setCourseInfo(temp, cId, stoi(cHour), cGrade[0]);
            } catch (const exception& e) {
                throw runtime_error("Invalid course data: " + string(e.what()));
            }
        }
        index++;
    }
}

void StudentType::getHoursEnrolled(){
    for(int i=0;i<Size;i++){
        for(int j=0;j<numberOfCourses[i];j++){
            totalHours[i]+=course[i][j].getCredits();
        }
    }

}

void StudentType::getGPA() {
    for (int i = 0; i < Size; i++) {
        if (numberOfCourses[i] == 0) {
            GPA[i] = 0.0;
            continue;
        }
        
        double totalPoints = 0.0;
        int totalCredits = 0;
        
        for (int j = 0; j < numberOfCourses[i]; j++) {
            char grade = course[i][j].getCourseGrade();
            int credits = course[i][j].getCredits();
            
            switch(grade) {
                case 'A': totalPoints += 4.0 * credits; break;
                case 'B': totalPoints += 3.0 * credits; break;
                case 'C': totalPoints += 2.0 * credits; break;
                case 'D': totalPoints += 1.0 * credits; break;
                case 'F': break;
                default: continue;
            }
            totalCredits += credits;
        }
        
        GPA[i] = totalCredits > 0 ? totalPoints / totalCredits : 0.0;
    }
}

void StudentType::billingAmmount(){
    for(int i=0;i<Size;i++){
        bill[i]=totalHours[i]*Ucret;
    }
}

void StudentType::printSortingResults(int studentIndex, bool toFile, ofstream* outputFile) {
    if (!showSortingResults) return;

    ostream& out = toFile ? *outputFile : cout;

    // Store current course arrangement
    CourseType* tempCourses = new CourseType[numberOfCourses[studentIndex]];
    for (int j = 0; j < numberOfCourses[studentIndex]; j++) {
        tempCourses[j] = course[studentIndex][j];
    }

    out << "\nSorting Results for Student " << (studentIndex + 1) << ":" << endl;

    // Sort and print by Grade
    out << "\nSorted by Grade:" << endl;
    out << "Course No\tCourse Name\tCredits\tGrade" << endl;
    out << "----------------------------------------" << endl;
    sortCoursesByGrade(studentIndex);
    for (int j = 0; j < numberOfCourses[studentIndex]; j++) {
        out << course[studentIndex][j].getCourseNo() << "\t"
            << course[studentIndex][j].getName() << "\t"
            << course[studentIndex][j].getCredits() << "\t";
        if (isTuitonPaid[studentIndex] == 'Y') {
            out << course[studentIndex][j].getCourseGrade();
        } else {
            out << "Not Paid";
        }
        out << endl;
    }

    // Sort and print by Credits
    out << "\nSorted by Credits:" << endl;
    out << "Course No\tCourse Name\tCredits\tGrade" << endl;
    out << "----------------------------------------" << endl;
    sortCoursesByCredit(studentIndex);
    for (int j = 0; j < numberOfCourses[studentIndex]; j++) {
        out << course[studentIndex][j].getCourseNo() << "\t"
            << course[studentIndex][j].getName() << "\t"
            << course[studentIndex][j].getCredits() << "\t";
        if (isTuitonPaid[studentIndex] == 'Y') {
            out << course[studentIndex][j].getCourseGrade();
        } else {
            out << "Not Paid";
        }
        out << endl;
    }

    // Sort and print by Course Name
    out << "\nSorted by Course Name:" << endl;
    out << "Course No\tCourse Name\tCredits\tGrade" << endl;
    out << "----------------------------------------" << endl;
    sortCoursesByName(studentIndex);
    for (int j = 0; j < numberOfCourses[studentIndex]; j++) {
        out << course[studentIndex][j].getCourseNo() << "\t"
            << course[studentIndex][j].getName() << "\t"
            << course[studentIndex][j].getCredits() << "\t";
        if (isTuitonPaid[studentIndex] == 'Y') {
            out << course[studentIndex][j].getCourseGrade();
        } else {
            out << "Not Paid";
        }
        out << endl;
    }

    // Restore original course arrangement
    for (int j = 0; j < numberOfCourses[studentIndex]; j++) {
        course[studentIndex][j] = tempCourses[j];
    }
    delete[] tempCourses;

    out << "\n================================================\n" << endl;
}

void StudentType::askAndShowSortingResults() {
    cout << "\nWould you like to see sorting results for courses? (Y/N): ";
    char choice;
    cin >> choice;
    showSortingResults = (choice == 'Y' || choice == 'y');
    
    if (showSortingResults) {
        // Print sorting results to console
        for (int i = 0; i < Size; i++) {
            printSortingResults(i, false);
        }
        
        // Print sorting results to file
        string outputFile = "output.txt";
        ofstream outFile(outputFile, ios::app);  // Open in append mode
        if (outFile.is_open()) {
            for (int i = 0; i < Size; i++) {
                printSortingResults(i, true, &outFile);
            }
            outFile.close();
        }
    }
}

void StudentType::print() {
    for (int i = 0; i < Size; i++) {
        cout << "Student Name: " << person[i].getName() << endl;
        cout << "Student ID: " << Id[i] << endl;
        cout << "Number of Courses Enrolled: " << numberOfCourses[i] << endl;
        cout << "Course No\tCourse Name\tCredits\tGrade" << endl;
        cout << "----------------------------------------" << endl;
        for (int j = 0; j < numberOfCourses[i]; j++) {
            cout << course[i][j].getCourseNo() << "\t"
                 << course[i][j].getName() << "\t"
                 << course[i][j].getCredits() << "\t";
            if (isTuitonPaid[i] == 'Y') {
                cout << course[i][j].getCourseGrade();
            } else {
                cout << "Not Paid";
            }
            cout << endl;
        }
        cout << "----------------------------------------" << endl;
        cout << "Total Number of Credits: " << totalHours[i] << endl;
        if (isTuitonPaid[i] == 'Y') {
            cout << "Mid-Semester GPA: " << setprecision(3) << GPA[i] << endl;
        } else {
            cout << "Mid-Semester GPA: Tuition fee not paid yet!" << endl;
        }
        cout << endl;
    }
}

void StudentType::print(string a) {
    ofstream outputFile(a);
    if (!outputFile.is_open()) {
        cout << "Could not open file: " << a << endl;
        return;
    }
    for (int i = 0; i < Size; i++) {
        outputFile << "Student Name: " << person[i].getName() << endl;
        outputFile << "Student ID: " << Id[i] << endl;
        outputFile << "Number of Courses Enrolled: " << numberOfCourses[i] << endl;
        outputFile << "Course No\tCourse Name\tCredits\tGrade" << endl;
        outputFile << "----------------------------------------" << endl;
        
        for (int j = 0; j < numberOfCourses[i]; j++) {
            outputFile << course[i][j].getCourseNo() << "\t"
                      << course[i][j].getName() << "\t"
                      << course[i][j].getCredits() << "\t";
            if (isTuitonPaid[i] == 'Y') {
                outputFile << course[i][j].getCourseGrade();
            } else {
                outputFile << "Not Paid";
            }
            outputFile << endl;
        }
        outputFile << "----------------------------------------" << endl;
        outputFile << "Total Number of Credits: " << totalHours[i] << endl;
        if (isTuitonPaid[i] == 'Y') {
            outputFile << "Mid-Semester GPA: " << setprecision(3) << GPA[i] << endl;
        } else {
            outputFile << "Mid-Semester GPA: Tuition fee not paid yet!" << endl;
        }
        outputFile << endl;
    }
    outputFile.close();
}

int checkSize(string filename){
    ifstream input(filename);
    while(!input.is_open()){
        cout<<"Dosya acilamadi: "<<filename<<endl;
        return 0;
    }
    int ogrenciSayisi;
    string text;
    getline(input,text);
    stringstream ss(text);
    ss>>ogrenciSayisi;
    input.close();
    return ogrenciSayisi;
}

void StudentType::sortCoursesByGrade(int studentIndex) {
    if (studentIndex < 0 || studentIndex >= Size) {
        throw invalid_argument("Invalid student index");
    }

    for (int i = 0; i < numberOfCourses[studentIndex] - 1; i++) {
        for (int j = 0; j < numberOfCourses[studentIndex] - i - 1; j++) {
            if (course[studentIndex][j].getCourseGrade() > course[studentIndex][j + 1].getCourseGrade()) {
                CourseType temp = course[studentIndex][j];
                course[studentIndex][j] = course[studentIndex][j + 1];
                course[studentIndex][j + 1] = temp;
            }
        }
    }
}

void StudentType::sortCoursesByCredit(int studentIndex) {
    if (studentIndex < 0 || studentIndex >= Size) {
        throw invalid_argument("Invalid student index");
    }

    for (int i = 0; i < numberOfCourses[studentIndex] - 1; i++) {
        for (int j = 0; j < numberOfCourses[studentIndex] - i - 1; j++) {
            if (course[studentIndex][j].getCredits() > course[studentIndex][j + 1].getCredits()) {
                CourseType temp = course[studentIndex][j];
                course[studentIndex][j] = course[studentIndex][j + 1];
                course[studentIndex][j + 1] = temp;
            }
        }
    }
}

void StudentType::sortCoursesByName(int studentIndex) {
    if (studentIndex < 0 || studentIndex >= Size) {
        throw invalid_argument("Invalid student index");
    }

    for (int i = 0; i < numberOfCourses[studentIndex] - 1; i++) {
        for (int j = 0; j < numberOfCourses[studentIndex] - i - 1; j++) {
            if (course[studentIndex][j].getName() > course[studentIndex][j + 1].getName()) {
                CourseType temp = course[studentIndex][j];
                course[studentIndex][j] = course[studentIndex][j + 1];
                course[studentIndex][j + 1] = temp;
            }
        }
    }
}

void StudentType::printSortedCourses(int studentIndex) {
    if (studentIndex < 0 || studentIndex >= Size) {
        throw invalid_argument("Invalid student index");
    }

    cout << "\nStudent: " << person[studentIndex].getName() << endl;
    cout << "Courses:" << endl;
    cout << "Course No\tCourse Name\tCredits\tGrade" << endl;
    cout << "----------------------------------------" << endl;

    for (int i = 0; i < numberOfCourses[studentIndex]; i++) {
        cout << course[studentIndex][i].getCourseNo() << "\t"
             << course[studentIndex][i].getName() << "\t"
             << course[studentIndex][i].getCredits() << "\t";
        if (isTuitonPaid[studentIndex] == 'Y') {
            cout << course[studentIndex][i].getCourseGrade();
        } else {
            cout << "Not Paid";
        }
        cout << endl;
    }
    cout << "----------------------------------------" << endl;
}

void StudentType::printStudentDetails(int studentIndex, bool toFile, ofstream* outputFile) {
    if (studentIndex < 0 || studentIndex >= Size) {
        if (!toFile) {
            cout << "Invalid student index!" << endl;
        }
        return;
    }

    ostream& out = toFile ? *outputFile : cout;

    out << "\n========== Student Details ==========" << endl;
    out << "Student Name: " << person[studentIndex].getName() << endl;
    out << "Student ID: " << Id[studentIndex] << endl;
    out << "Number of Courses Enrolled: " << numberOfCourses[studentIndex] << endl;
    out << "Course No\tCourse Name\tCredits\tGrade" << endl;
    out << "----------------------------------------" << endl;
    
    for (int j = 0; j < numberOfCourses[studentIndex]; j++) {
        out << course[studentIndex][j].getCourseNo() << "\t"
            << course[studentIndex][j].getName() << "\t"
            << course[studentIndex][j].getCredits() << "\t";
        if (isTuitonPaid[studentIndex] == 'Y') {
            out << course[studentIndex][j].getCourseGrade();
        } else {
            out << "Not Paid";
        }
        out << endl;
    }
    
    out << "----------------------------------------" << endl;
    out << "Total Number of Credits: " << totalHours[studentIndex] << endl;
    if (isTuitonPaid[studentIndex] == 'Y') {
        out << "Mid-Semester GPA: " << setprecision(3) << GPA[studentIndex] << endl;
    } else {
        out << "Mid-Semester GPA: Tuition fee not paid yet!" << endl;
    }
    out << "========================================" << endl;
}

void StudentType::writeSearchResultsToFile(const vector<int>& matches, int selectedIndex) {
    ofstream outputFile("output.txt", ios::app);  // Open in append mode
    if (!outputFile.is_open()) {
        cout << "Could not open output file for writing search results!" << endl;
        return;
    }

    // Clear any existing content in the file
    outputFile.seekp(0);
    outputFile << "=========== STUDENT REPORT SYSTEM ===========" << endl << endl;

    // Write all students first (like print function does)
    for (int i = 0; i < Size; i++) {
        outputFile << "Student Name: " << person[i].getName() << endl;
        outputFile << "Student ID: " << Id[i] << endl;
        outputFile << "Number of Courses Enrolled: " << numberOfCourses[i] << endl;
        outputFile << "Course No\tCourse Name\tCredits\tGrade" << endl;
        outputFile << "----------------------------------------" << endl;
        
        for (int j = 0; j < numberOfCourses[i]; j++) {
            outputFile << course[i][j].getCourseNo() << "\t"
                      << course[i][j].getName() << "\t"
                      << course[i][j].getCredits() << "\t";
            if (isTuitonPaid[i] == 'Y') {
                outputFile << course[i][j].getCourseGrade();
            } else {
                outputFile << "Not Paid";
            }
            outputFile << endl;
        }
        outputFile << "----------------------------------------" << endl;
        outputFile << "Total Number of Credits: " << totalHours[i] << endl;
        if (isTuitonPaid[i] == 'Y') {
            outputFile << "Mid-Semester GPA: " << setprecision(3) << GPA[i] << endl;
        } else {
            outputFile << "Mid-Semester GPA: Tuition fee not paid yet!" << endl;
        }
        outputFile << endl;
    }

    // Write search results
    outputFile << "\n============ SEARCH RESULTS ============" << endl;
    if (matches.size() > 1) {
        outputFile << "Found " << matches.size() << " matching students:" << endl;
        for (size_t i = 0; i < matches.size(); i++) {
            outputFile << i + 1 << ". " << person[matches[i]].getName() 
                      << " (ID: " << Id[matches[i]] << ")" << endl;
        }
        outputFile << "\nSelected student details:" << endl;
    }

    // Print details of selected student
    outputFile << "\n========== Student Details ==========" << endl;
    outputFile << "Student Name: " << person[selectedIndex].getName() << endl;
    outputFile << "Student ID: " << Id[selectedIndex] << endl;
    outputFile << "Number of Courses Enrolled: " << numberOfCourses[selectedIndex] << endl;
    outputFile << "Course No\tCourse Name\tCredits\tGrade" << endl;
    outputFile << "----------------------------------------" << endl;
    
    for (int j = 0; j < numberOfCourses[selectedIndex]; j++) {
        outputFile << course[selectedIndex][j].getCourseNo() << "\t"
                  << course[selectedIndex][j].getName() << "\t"
                  << course[selectedIndex][j].getCredits() << "\t";
        if (isTuitonPaid[selectedIndex] == 'Y') {
            outputFile << course[selectedIndex][j].getCourseGrade();
        } else {
            outputFile << "Not Paid";
        }
        outputFile << endl;
    }
    
    outputFile << "----------------------------------------" << endl;
    outputFile << "Total Number of Credits: " << totalHours[selectedIndex] << endl;
    if (isTuitonPaid[selectedIndex] == 'Y') {
        outputFile << "Mid-Semester GPA: " << setprecision(3) << GPA[selectedIndex] << endl;
    } else {
        outputFile << "Mid-Semester GPA: Tuition fee not paid yet!" << endl;
    }
    outputFile << "========================================" << endl;

    outputFile.close();
}

void StudentType::searchStudent() {
    while (true) {
        cout << "\nSearch Options:" << endl;
        cout << "1. Search by Name" << endl;
        cout << "2. Search by ID" << endl;
        cout << "3. Return to Main Menu" << endl;
        cout << "Enter your choice (1-3): ";
        
        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number." << endl;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (choice == 3) return;
        
        if (choice != 1 && choice != 2) {
            cout << "Invalid choice! Please enter 1, 2, or 3." << endl;
            continue;
        }
        
        string searchTerm;
        bool found = false;
        vector<int> matches;
        
        switch(choice) {
            case 1: {
                cout << "Enter student name (case sensitive): ";
                getline(cin, searchTerm);
                
                if (searchTerm.empty()) {
                    cout << "Name cannot be empty!" << endl;
                    continue;
                }
                
                for (int i = 0; i < Size; i++) {
                    if (person[i].getName().find(searchTerm) != string::npos) {
                        matches.push_back(i);
                        found = true;
                    }
                }
                break;
            }
            case 2: {
                cout << "Enter student ID: ";
                getline(cin, searchTerm);
                
                if (searchTerm.empty()) {
                    cout << "ID cannot be empty!" << endl;
                    continue;
                }
                
                for (int i = 0; i < Size; i++) {
                    if (Id[i] == searchTerm) {
                        matches.push_back(i);
                        found = true;
                    }
                }
                break;
            }
        }
        
        if (!found) {
            cout << "No matching students found!" << endl;
            
            // Write to file that no matches were found
            ofstream outputFile("output.txt");
            if (outputFile.is_open()) {
                outputFile << "=========== STUDENT REPORT SYSTEM ===========" << endl << endl;
                print(string("output.txt")); // Write all students first
                
                outputFile.open("output.txt", ios::app);
                outputFile << "\n============ SEARCH RESULTS ============" << endl;
                outputFile << "Search term: " << searchTerm << endl;
                outputFile << "No matching students found!" << endl;
                outputFile << "=======================================" << endl;
                outputFile.close();
            }
            continue;
        }
        
        if (matches.size() == 1) {
            printStudentDetails(matches[0], false);
            writeSearchResultsToFile(matches, matches[0]);
            
            // Ask for sorting results
            while (true) {
                cout << "\nWould you like to see sorting results for this student? (Y/N): ";
                string sortChoice;
                getline(cin, sortChoice);
                
                if (sortChoice.empty()) {
                    cout << "Invalid input! Please enter Y or N." << endl;
                    continue;
                }
                
                char firstChar = toupper(sortChoice[0]);
                if (firstChar == 'Y') {
                    printSortingResults(matches[0], false);
                    // Add sorting results to file
                    ofstream outFile("output.txt", ios::app);
                    if (outFile.is_open()) {
                        outFile << "\n============ SORTING RESULTS ============" << endl;
                        printSortingResults(matches[0], true, &outFile);
                        outFile.close();
                    }
                    break;
                } else if (firstChar == 'N') {
                    break;
                } else {
                    cout << "Invalid input! Please enter Y or N." << endl;
                }
            }
        } else {
            while (true) {
                cout << "\nMultiple matches found. Please select a student:" << endl;
                for (size_t i = 0; i < matches.size(); i++) {
                    cout << i + 1 << ". " << person[matches[i]].getName() 
                         << " (ID: " << Id[matches[i]] << ")" << endl;
                }
                cout << matches.size() + 1 << ". Return to search menu" << endl;
                
                cout << "Enter your choice (1-" << matches.size() + 1 << "): ";
                int selection;
                
                if (!(cin >> selection)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input! Please enter a number." << endl;
                    continue;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                if (selection == static_cast<int>(matches.size()) + 1) {
                    break;
                }
                
                if (selection >= 1 && selection <= static_cast<int>(matches.size())) {
                    int selectedIndex = matches[selection - 1];
                    printStudentDetails(selectedIndex, false);
                    writeSearchResultsToFile(matches, selectedIndex);
                    
                    // Ask for sorting results
                    while (true) {
                        cout << "\nWould you like to see sorting results for this student? (Y/N): ";
                        string sortChoice;
                        getline(cin, sortChoice);
                        
                        if (sortChoice.empty()) {
                            cout << "Invalid input! Please enter Y or N." << endl;
                            continue;
                        }
                        
                        char firstChar = toupper(sortChoice[0]);
                        if (firstChar == 'Y') {
                            printSortingResults(selectedIndex, false);
                            // Add sorting results to file
                            ofstream outFile("output.txt", ios::app);
                            if (outFile.is_open()) {
                                outFile << "\n============ SORTING RESULTS ============" << endl;
                                printSortingResults(selectedIndex, true, &outFile);
                                outFile.close();
                            }
                            break;
                        } else if (firstChar == 'N') {
                            break;
                        } else {
                            cout << "Invalid input! Please enter Y or N." << endl;
                        }
                    }
                    break;
                } else {
                    cout << "Invalid selection! Please try again." << endl;
                }
            }
        }
    }
}

int main() {
    try {
        string inputFile = "input.txt";
        string outputFile = "output.txt";
        
        StudentType student(checkSize(inputFile));
        student.readInput(inputFile);
        student.getHoursEnrolled();
        student.getGPA();
        student.billingAmmount();
        
        while (true) {
            cout << "\nStudent Report System Menu:" << endl;
            cout << "1. Show All Students" << endl;
            cout << "2. Search Student" << endl;
            cout << "3. Show Sorting Results" << endl;
            cout << "4. Exit" << endl;
            cout << "Enter your choice (1-4): ";
            
            int choice;
            if (!(cin >> choice)) {
                cin.clear(); // Clear error flags
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
                cout << "Invalid input! Please enter a number." << endl;
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear any remaining newline
            
            switch(choice) {
                case 1:
                    student.print();
                    student.print(outputFile);
                    break;
                case 2:
                    student.searchStudent();
                    break;
                case 3:
                    student.askAndShowSortingResults();
                    break;
                case 4:
                    cout << "Exiting program..." << endl;
                    return 0;
                default:
                    cout << "Invalid choice! Please enter a number between 1 and 4." << endl;
            }
        }
        
        return 0;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
}