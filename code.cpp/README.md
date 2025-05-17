# Student Report System

A comprehensive C++ program for managing student academic records and generating detailed reports. This system allows for efficient tracking of student information, course details, grades, and provides various sorting and searching functionalities.

## Features

- **Student Information Management**
  - Store and manage student personal details
  - Track course enrollments and grades
  - Calculate GPA and total credit hours
  - Handle tuition payment status

- **Course Management**
  - Store course details (name, number, credits)
  - Track course grades
  - Support multiple courses per student

- **Search Capabilities**
  - Search students by name
  - Search students by ID
  - Display detailed student information

- **Sorting Functions**
  - Sort courses by grade
  - Sort courses by credit hours
  - Sort courses by course name

- **Report Generation**
  - Generate comprehensive student reports
  - Export results to output file
  - Display sorting results
  - Save search results

## System Requirements

- C++ compiler with C++11 support
- Standard input/output streams
- File system access for reading input and writing output

## File Structure

- `report.cpp` - Main source code file
- `input.txt` - Input file containing student data
- `output.txt` - Generated report file (created by program)

## Input File Format

The input file (`input.txt`) should follow this format:

```
<number_of_students> <credit_hour_fee>
<student_first_name> <student_last_name> <student_id> <tuition_status> <number_of_courses>
<course_name> <course_id> <credit_hours> <grade>
...
```

Example:
```
2 100
John Doe 12345 Y 2
Mathematics MATH101 3 A
Physics PHY101 4 B
Jane Smith 67890 N 1
Chemistry CHEM101 3 A
```

## Usage

1. Compile the program:
   ```bash
   g++ report.cpp -o student_report
   ```

2. Prepare your input file (`input.txt`) with the correct format

3. Run the program:
   ```bash
   ./student_report
   ```

4. Use the menu to:
   - View all student records
   - Search for specific students
   - View sorted course information
   - Generate reports

## Menu Options

1. **Show All Students**
   - Displays complete information for all students
   - Automatically saves to output.txt

2. **Search Student**
   - Search by name (case sensitive)
   - Search by ID
   - View detailed student information
   - Option to view sorting results

3. **Show Sorting Results**
   - View courses sorted by different criteria
   - Save sorting results to output.txt

4. **Exit**
   - Safely exit the program

## Error Handling

The program includes comprehensive error handling for:
- File operations
- Invalid input data
- Memory management
- User input validation
- Search operations

## Classes

### PersonType
- Basic person information management
- Name storage and retrieval

### CourseType
- Course information management
- Grade and credit handling
- Course validation

### StudentType
- Main class for student management
- Handles all student-related operations
- Manages course lists and calculations

## Contributing

Feel free to contribute to this project by:
1. Forking the repository
2. Creating your feature branch
3. Committing your changes
4. Pushing to the branch
5. Creating a new Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Thanks to all contributors who have helped with testing and improvements
- Special thanks to the C++ community for valuable resources and support 