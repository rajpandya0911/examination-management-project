# Examination Management System (C++11)

## Project files
- `main.cpp` — complete source code; includes the 15 automated self-tests and text UML notes.
- `README.md` — build/run instructions and project overview.
- `test_cases.md` — 15 documented test cases.
- `requirements_checklist.md` — mapping to the assignment checklist.
- `diagrams/use_case_diagram.svg` — use case diagram.
- `diagrams/class_diagram.svg` — class diagram.
- `diagrams/sequence_add_student.svg` — sequence diagram for adding a student.
- `diagrams/sequence_search_student.svg` — sequence diagram for searching a student.
- `exam_system` / `exam_system.exe` — generated only if a suitable compiler is available in the environment.

## Requirements
The program uses five classes: `Person`, `ExamResult`, `Student`, `FileManager`, and `ExaminationManager`.
It demonstrates constructors/destructors, inheritance, runtime polymorphism, dynamic memory (`shared_ptr`), file persistence, and Add/Search/Update/Delete operations.

## Compile
Linux/macOS with g++:
```bash
g++ -std=c++11 -Wall -Wextra -pedantic main.cpp -o exam_system
```

Windows with MinGW:
```bat
g++ -std=c++11 -Wall -Wextra -pedantic main.cpp -o exam_system.exe
```

## Run
```bash
./exam_system
```
On Windows, run:
```bat
exam_system.exe
```

## Run automated self-tests
```bash
./exam_system --test
```
The test mode runs 15 checks for mark validation, totals, grades, pass/fail, and mark updates.

## Data file
Records are saved in `exam_records.txt` in the directory where the program is run. The file is created when records are saved. Back up this file if you need to preserve records.

## UML diagrams
Open the SVG files in a browser or insert them into a report. They can also be converted to PNG using an SVG-capable graphics tool.
