/*
EXAMINATION MANAGEMENT SYSTEM - C++11
============================================================
README
------
Purpose:
  Manage student examination records and calculate total, percentage,
  grade, and pass/fail status.

Features:
  - Five classes: Person, ExamResult, Student, FileManager, ExaminationManager
  - Constructors and virtual destructors
  - Inheritance and runtime polymorphism
  - Dynamic memory using shared_ptr
  - File persistence in exam_records.txt
  - Add, search, update, delete, display-all, and display-results
  - Input validation
  - Built-in 15-case self-test mode
  - UML diagrams included below as comments

Compile:
  g++ -std=c++11 main.cpp -o exam_system

Run:
  ./exam_system
  ./exam_system --test

Windows (MinGW):
  g++ -std=c++11 main.cpp -o exam_system.exe
  exam_system.exe
  exam_system.exe --test

File format:
  Each record contains roll number, name, course, and five marks,
  each on a separate line.

USE CASE DIAGRAM (text UML)
---------------------------
              +--------------------------------------+
              |     Examination Management System    |
  Admin ----->| (Add Student)                         |
  Admin ----->| (Search Student)                      |
  Admin ----->| (Update Student)                      |
  Admin ----->| (Delete Student)                      |
  Admin ----->| (Display All Students)                |
  Admin ----->| (Display Examination Results)         |
              +--------------------------------------+

CLASS DIAGRAM (text UML)
------------------------
  Person (abstract)
    - name: string
    + display(): void [pure virtual]
          ^
          | inherits
  Student : Person
    - rollNo: int
    - course: string
    - result: ExamResult
    + display(): void
    + writeTo(ostream): void
    + readFrom(istream): bool

  ExamResult
    - marks[5]: float
    + total(), percentage(), grade(), passed()
    + input(), display(), writeTo(), readFrom()

  FileManager
    - filename: string
    + load(), saveAll()

  ExaminationManager
    - students: vector<shared_ptr<Student>>
    - fileManager: FileManager
    + addStudent(), searchStudent(), updateStudent()
    + deleteStudent(), displayAll(), displayResults(), run()

SEQUENCE DIAGRAM 1: ADD STUDENT
------------------------------
Admin -> ExaminationManager: addStudent()
ExaminationManager -> ExamResult: input marks
ExaminationManager -> Student: construct student
ExaminationManager -> FileManager: saveAll(records)
FileManager -> exam_records.txt: write records
FileManager --> ExaminationManager: save status
ExaminationManager --> Admin: show success/failure

SEQUENCE DIAGRAM 2: SEARCH STUDENT
----------------------------------
Admin -> ExaminationManager: searchStudent(roll)
ExaminationManager -> records: find roll number
records --> ExaminationManager: matching student / not found
ExaminationManager -> Student: display()
Student --> Admin: student details and result

15 TEST CASES (run with --test)
-------------------------------
TC01: Negative marks are rejected.
TC02: Marks above 100 are rejected.
TC03: Marks of 0 are accepted.
TC04: Marks of 100 are accepted.
TC05: Five zero marks calculate total 0.
TC06: Five 100 marks calculate total 500.
TC07: 90 percent receives grade A+.
TC08: 80 percent receives grade A.
TC09: 70 percent receives grade B.
TC10: 60 percent receives grade C.
TC11: 40 percent receives grade D.
TC12: 40 percent is a pass.
TC13: Below 40 percent is a fail.
TC14: A valid mark can be set and retrieved.
TC15: An invalid mark update is rejected.
The self-test runner reports PASS/FAIL for these 15 automated checks.
============================================================
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>
#include <limits>
#include <sstream>
#include <cstdio>

using namespace std;

static const int SUBJECT_COUNT = 5;

class Person {
protected:
    string name;

public:
    Person() : name("") {}
    explicit Person(const string& personName) : name(personName) {}
    virtual ~Person() {}

    string getName() const { return name; }
    void setName(const string& personName) { name = personName; }

    virtual void display() const = 0;
};

class ExamResult {
private:
    float marks[SUBJECT_COUNT];

public:
    ExamResult() {
        for (int i = 0; i < SUBJECT_COUNT; ++i) marks[i] = 0.0f;
    }

    explicit ExamResult(const float inputMarks[SUBJECT_COUNT]) {
        for (int i = 0; i < SUBJECT_COUNT; ++i) {
            marks[i] = isValidMark(inputMarks[i]) ? inputMarks[i] : 0.0f;
        }
    }

    ~ExamResult() {}

    static bool isValidMark(float mark) {
        return mark >= 0.0f && mark <= 100.0f;
    }

    bool setMark(int index, float value) {
        if (index < 0 || index >= SUBJECT_COUNT || !isValidMark(value))
            return false;
        marks[index] = value;
        return true;
    }

    float getMark(int index) const {
        if (index < 0 || index >= SUBJECT_COUNT) return 0.0f;
        return marks[index];
    }

    void input() {
        cout << "Enter marks for 5 subjects (0-100):\n";
        for (int i = 0; i < SUBJECT_COUNT; ++i) {
            while (true) {
                cout << "Subject " << (i + 1) << ": ";
                float value;
                if (cin >> value && isValidMark(value)) {
                    marks[i] = value;
                    break;
                }
                cout << "Invalid marks. Enter a number from 0 to 100.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    float total() const {
        float sum = 0.0f;
        for (int i = 0; i < SUBJECT_COUNT; ++i) sum += marks[i];
        return sum;
    }

    float percentage() const { return total() / SUBJECT_COUNT; }

    string grade() const {
        float p = percentage();
        if (p >= 90) return "A+";
        if (p >= 80) return "A";
        if (p >= 70) return "B";
        if (p >= 60) return "C";
        if (p >= 40) return "D";
        return "F";
    }

    bool passed() const { return percentage() >= 40.0f; }

    void display() const {
        cout << "Subject marks: ";
        for (int i = 0; i < SUBJECT_COUNT; ++i)
            cout << marks[i] << (i == SUBJECT_COUNT - 1 ? '\n' : ' ');
        cout << "Total       : " << total() << " / 500\n";
        cout << "Percentage  : " << fixed << setprecision(2) << percentage() << "%\n";
        cout << "Grade       : " << grade() << '\n';
        cout << "Result      : " << (passed() ? "PASS" : "FAIL") << '\n';
    }

    void writeTo(ostream& out) const {
        for (int i = 0; i < SUBJECT_COUNT; ++i) out << marks[i] << '\n';
    }

    bool readFrom(istream& in) {
        for (int i = 0; i < SUBJECT_COUNT; ++i) {
            if (!(in >> marks[i]) || !isValidMark(marks[i])) return false;
        }
        in.ignore(numeric_limits<streamsize>::max(), '\n');
        return true;
    }
};

class Student : public Person {
private:
    int rollNo;
    string course;
    ExamResult result;

public:
    Student() : Person(), rollNo(0), course("") {}

    Student(int roll, const string& studentName, const string& studentCourse,
            const ExamResult& examResult)
        : Person(studentName), rollNo(roll), course(studentCourse), result(examResult) {}

    virtual ~Student() {}

    int getRollNo() const { return rollNo; }
    string getCourse() const { return course; }
    const ExamResult& getResult() const { return result; }
    ExamResult& getResult() { return result; }

    void setRollNo(int roll) { rollNo = roll; }
    void setCourse(const string& studentCourse) { course = studentCourse; }

    virtual void display() const {
        cout << "\n--------------------------------------------\n";
        cout << "Roll number : " << rollNo << '\n';
        cout << "Student name: " << name << '\n';
        cout << "Course      : " << course << '\n';
        result.display();
        cout << "--------------------------------------------\n";
    }

    void writeTo(ostream& out) const {
        out << rollNo << '\n' << name << '\n' << course << '\n';
        result.writeTo(out);
    }

    bool readFrom(istream& in) {
        if (!(in >> rollNo)) return false;
        in.ignore(numeric_limits<streamsize>::max(), '\n');
        if (!getline(in, name) || !getline(in, course)) return false;
        if (rollNo <= 0 || name.empty() || course.empty()) return false;
        return result.readFrom(in);
    }
};

class FileManager {
private:
    string filename;

public:
    explicit FileManager(const string& file = "exam_records.txt") : filename(file) {}
    ~FileManager() {}

    bool load(vector<shared_ptr<Student> >& students) const {
        ifstream file(filename.c_str());
        if (!file.is_open()) return true; // No file is normal on first run.

        while (true) {
            shared_ptr<Student> student(new Student());
            if (student->readFrom(file)) {
                students.push_back(student);
            } else {
                break;
            }
        }
        return true;
    }

    bool saveAll(const vector<shared_ptr<Student> >& students) const {
        ofstream file(filename.c_str(), ios::trunc);
        if (!file.is_open()) {
            cerr << "Error: cannot write to " << filename << ".\n";
            return false;
        }
        for (size_t i = 0; i < students.size(); ++i)
            students[i]->writeTo(file);
        return file.good();
    }
};

class ExaminationManager {
private:
    vector<shared_ptr<Student> > students;
    FileManager fileManager;

    int findIndex(int rollNo) const {
        for (size_t i = 0; i < students.size(); ++i)
            if (students[i]->getRollNo() == rollNo) return static_cast<int>(i);
        return -1;
    }

    static int readPositiveInt(const string& prompt) {
        int value;
        while (true) {
            cout << prompt;
            if (cin >> value && value > 0) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
            cout << "Please enter a positive whole number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    static string readNonEmptyLine(const string& prompt) {
        string value;
        do {
            cout << prompt;
            getline(cin, value);
            if (value.empty()) cout << "This field cannot be empty.\n";
        } while (value.empty());
        return value;
    }

public:
    explicit ExaminationManager(const string& filename = "exam_records.txt")
        : students(), fileManager(filename) {
        fileManager.load(students);
    }

    ~ExaminationManager() {}

    bool addRecord(const Student& student) {
        if (student.getRollNo() <= 0 || student.getName().empty() ||
            student.getCourse().empty() || findIndex(student.getRollNo()) != -1)
            return false;
        students.push_back(shared_ptr<Student>(new Student(student)));
        return fileManager.saveAll(students);
    }

    bool searchRecord(int rollNo, Student& found) const {
        int index = findIndex(rollNo);
        if (index == -1) return false;
        found = *students[index];
        return true;
    }

    bool deleteRecord(int rollNo) {
        int index = findIndex(rollNo);
        if (index == -1) return false;
        students.erase(students.begin() + index);
        return fileManager.saveAll(students);
    }

    bool updateRecord(int rollNo, const Student& replacement) {
        int index = findIndex(rollNo);
        if (index == -1 || replacement.getName().empty() ||
            replacement.getCourse().empty()) return false;
        Student updated(replacement);
        updated.setRollNo(rollNo);
        *students[index] = updated;
        return fileManager.saveAll(students);
    }

    void addStudent() {
        int roll = readPositiveInt("Enter roll number: ");
        if (findIndex(roll) != -1) {
            cout << "That roll number already exists. Record not added.\n";
            return;
        }
        string studentName = readNonEmptyLine("Enter student name: ");
        string course = readNonEmptyLine("Enter course name: ");
        ExamResult exam;
        exam.input();
        Student student(roll, studentName, course, exam);
        if (addRecord(student))
            cout << "Student added and saved successfully.\n";
        else
            cout << "Could not save student record.\n";
    }

    void displayAll() const {
        if (students.empty()) {
            cout << "No student records available.\n";
            return;
        }
        // Runtime polymorphism: call overridden display through Person pointer.
        for (size_t i = 0; i < students.size(); ++i) {
            const Person* person = students[i].get();
            person->display();
        }
    }

    void searchStudent() const {
        int roll = readPositiveInt("Enter roll number to search: ");
        int index = findIndex(roll);
        if (index == -1) cout << "Student not found.\n";
        else students[index]->display();
    }

    void updateStudent() {
        int roll = readPositiveInt("Enter roll number to update: ");
        int index = findIndex(roll);
        if (index == -1) {
            cout << "Student not found.\n";
            return;
        }
        string studentName = readNonEmptyLine("New student name: ");
        string course = readNonEmptyLine("New course name: ");
        ExamResult exam;
        exam.input();
        Student replacement(roll, studentName, course, exam);
        if (updateRecord(roll, replacement))
            cout << "Student record updated successfully.\n";
        else
            cout << "Could not update student record.\n";
    }

    void deleteStudent() {
        int roll = readPositiveInt("Enter roll number to delete: ");
        if (deleteRecord(roll))
            cout << "Student record deleted successfully.\n";
        else
            cout << "Student not found or file save failed.\n";
    }

    void displayResults() const {
        if (students.empty()) {
            cout << "No examination results available.\n";
            return;
        }
        cout << "\n========== EXAMINATION RESULTS ==========\n";
        cout << left << setw(10) << "Roll" << setw(24) << "Name"
             << setw(12) << "Percent" << setw(8) << "Grade" << "Result\n";
        cout << string(65, '-') << '\n';
        for (size_t i = 0; i < students.size(); ++i) {
            const Student& s = *students[i];
            cout << left << setw(10) << s.getRollNo()
                 << setw(24) << s.getName().substr(0, 22)
                 << setw(12) << fixed << setprecision(2)
                 << s.getResult().percentage()
                 << setw(8) << s.getResult().grade()
                 << (s.getResult().passed() ? "PASS" : "FAIL") << '\n';
        }
    }

    void run() {
        int choice = 0;
        do {
            cout << "\n============================================\n";
            cout << "       EXAMINATION MANAGEMENT SYSTEM\n";
            cout << "============================================\n";
            cout << "1. Add student record\n";
            cout << "2. Display all students\n";
            cout << "3. Search student by roll number\n";
            cout << "4. Update student record\n";
            cout << "5. Delete student record\n";
            cout << "6. Display examination results\n";
            cout << "7. Exit\n";
            cout << "Enter choice: ";

            if (!(cin >> choice)) {
                cout << "Invalid input. Enter a menu number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice) {
                case 1: addStudent(); break;
                case 2: displayAll(); break;
                case 3: searchStudent(); break;
                case 4: updateStudent(); break;
                case 5: deleteStudent(); break;
                case 6: displayResults(); break;
                case 7: cout << "Records are stored in exam_records.txt. Goodbye!\n"; break;
                default: cout << "Invalid choice. Select 1 to 7.\n";
            }
        } while (choice != 7);
    }
};

static bool runSelfTests() {
    int passedCount = 0;
    int failedCount = 0;
    int testNo = 0;

    // Helper reports one test result.
    struct TestReporter {
        static void check(bool condition, const string& description,
                          int& number, int& passed, int& failed) {
            ++number;
            cout << "TC" << setw(2) << setfill('0') << number << setfill(' ')
                 << ": " << description << " -> "
                 << (condition ? "PASS" : "FAIL") << '\n';
            if (condition) ++passed;
            else ++failed;
        }
    };

    float zeros[5] = {0, 0, 0, 0, 0};
    float hundreds[5] = {100, 100, 100, 100, 100};
    float ninety[5] = {90, 90, 90, 90, 90};
    float eighty[5] = {80, 80, 80, 80, 80};
    float seventy[5] = {70, 70, 70, 70, 70};
    float sixty[5] = {60, 60, 60, 60, 60};
    float forty[5] = {40, 40, 40, 40, 40};
    float thirtyNine[5] = {39, 39, 39, 39, 39};

    ExamResult r0(zeros), r100(hundreds), r90(ninety), r80(eighty);
    ExamResult r70(seventy), r60(sixty), r40(forty), r39(thirtyNine);
    ExamResult validation;

    TestReporter::check(!ExamResult::isValidMark(-1), "Reject negative marks",
                        testNo, passedCount, failedCount);
    TestReporter::check(!ExamResult::isValidMark(101), "Reject marks above 100",
                        testNo, passedCount, failedCount);
    TestReporter::check(ExamResult::isValidMark(0), "Accept mark 0",
                        testNo, passedCount, failedCount);
    TestReporter::check(ExamResult::isValidMark(100), "Accept mark 100",
                        testNo, passedCount, failedCount);
    TestReporter::check(r0.total() == 0, "Five zero marks total 0",
                        testNo, passedCount, failedCount);
    TestReporter::check(r100.total() == 500, "Five 100 marks total 500",
                        testNo, passedCount, failedCount);
    TestReporter::check(r90.grade() == "A+", "90 percent gives A+",
                        testNo, passedCount, failedCount);
    TestReporter::check(r80.grade() == "A", "80 percent gives A",
                        testNo, passedCount, failedCount);
    TestReporter::check(r70.grade() == "B", "70 percent gives B",
                        testNo, passedCount, failedCount);
    TestReporter::check(r60.grade() == "C", "60 percent gives C",
                        testNo, passedCount, failedCount);
    TestReporter::check(r40.grade() == "D", "40 percent gives D",
                        testNo, passedCount, failedCount);
    TestReporter::check(r40.passed(), "40 percent is a pass",
                        testNo, passedCount, failedCount);
    TestReporter::check(!r39.passed(), "Below 40 percent is a fail",
                        testNo, passedCount, failedCount);
    TestReporter::check(validation.setMark(0, 55) && validation.getMark(0) == 55,
                        "Set and retrieve a valid mark",
                        testNo, passedCount, failedCount);
    TestReporter::check(!validation.setMark(0, 120),
                        "Reject invalid mark when updating",
                        testNo, passedCount, failedCount);

    cout << "\nSelf-test summary: " << passedCount << " passed, "
         << failedCount << " failed out of " << testNo << ".\n";
    return failedCount == 0;
}

int main(int argc, char* argv[]) {
    if (argc > 1 && string(argv[1]) == "--test")
        return runSelfTests() ? 0 : 1;

    ExaminationManager manager;
    manager.run();
    return 0;
}
