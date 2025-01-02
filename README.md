# Student Report System

A C++ project designed to generate student reports by processing data such as tuition status, enrolled courses, and grades. This system uses object-oriented programming principles like classes, inheritance, and composition to create detailed and accurate reports.

## Features
- Calculates GPA based on course grades.
- Generates a detailed report for each student, including:
  - Tuition payment status.
  - Credit and fee calculations.
  - Individual course details.
- Handles scenarios for both paid and unpaid tuition.
- Modular and reusable code using OOP principles.

## Technologies
- **Language**: C++
- **Concepts**: Classes, Inheritance, Composition, File I/O

## Project Structure
StudentReportSystem/
├── README.md               # Project overview  
├── Program_description.pdf # Documentation  
├── student_class_details.cpp # Source code file  

## Getting Started
1. Clone the repository:  
   `git clone https://github.com/AlaattinUysal/StudentReportSystem.git`  
2. Compile the code:  
   `g++ student_class_details.cpp -o student_report`  
3. Run the program:  
   `./student_report`  

## Input Format
The first line contains the number of students and tuition fee per credit hour. The subsequent lines list the details of each student, including their enrolled courses and grades.  

**Example Input:**  
3 150
John Doe 3
Math 3 A
Physics 4 B
Chemistry 3 A
Jane Smith 2
Biology 4 B
History 3 A
Mark Lee 1
Art 2 C

## Output Example
Student: John Doe
GPA: 3.67
Total Credits: 10
Tuition Fee: $1500
Status: Paid

## Contribution
Contributions are welcome! Feel free to fork this repository and submit a pull request.

## Contact
For any questions or feedback, feel free to contact me:  
- **Email**: alaaddinuysal9@gmail.com  

