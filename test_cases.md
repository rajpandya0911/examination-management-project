# Test Cases — Examination Management System

Run automated checks with `./exam_system --test` after compiling. The program also needs manual menu testing for CRUD and file persistence.

| ID | Test scenario | Input / setup | Expected result | Automated in `--test`? |
|---|---|---|---|---|
| TC01 | Reject negative marks | `-1` | Mark is invalid | Yes |
| TC02 | Reject marks above range | `101` | Mark is invalid | Yes |
| TC03 | Accept minimum mark | `0` | Mark is valid | Yes |
| TC04 | Accept maximum mark | `100` | Mark is valid | Yes |
| TC05 | Calculate zero total | Five marks of `0` | Total = 0 | Yes |
| TC06 | Calculate maximum total | Five marks of `100` | Total = 500 | Yes |
| TC07 | Grade A+ boundary | Five marks of `90` | Percentage 90; grade A+ | Yes |
| TC08 | Grade A boundary | Five marks of `80` | Percentage 80; grade A | Yes |
| TC09 | Grade B boundary | Five marks of `70` | Percentage 70; grade B | Yes |
| TC10 | Grade C boundary | Five marks of `60` | Percentage 60; grade C | Yes |
| TC11 | Grade D boundary | Five marks of `40` | Percentage 40; grade D | Yes |
| TC12 | Pass boundary | Five marks of `40` | Result PASS | Yes |
| TC13 | Fail below boundary | Five marks of `39` | Result FAIL | Yes |
| TC14 | Update a valid mark | Set subject 1 to `55` | Stored mark becomes 55 | Yes |
| TC15 | Reject invalid mark update | Set subject 1 to `120` | Update rejected; previous mark retained | Yes |
| MT01 | Add a new student | Unique positive roll, non-empty name/course, five marks | Student appears in list and is saved | Manual |
| MT02 | Prevent duplicate roll number | Add another student using an existing roll | Duplicate is rejected | Manual |
| MT03 | Search existing student | Enter a saved roll number | Matching record is displayed | Manual |
| MT04 | Search missing student | Enter a roll not in records | “Student not found” is displayed | Manual |
| MT05 | Update student | Change name/course/marks for existing roll | Updated values are shown and saved | Manual |
| MT06 | Delete student | Delete an existing roll | Record disappears from list and file | Manual |
| MT07 | File persistence | Add record, exit, restart | Record remains available | Manual |
| MT08 | Empty record list | Start with no saved records | Display operation reports no records | Manual |

Note: The checklist requires at least 15 test cases. TC01–TC15 are the 15 automated checks; additional manual scenarios are included to test the full menu and file operations.
