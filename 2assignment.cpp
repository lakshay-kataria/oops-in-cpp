#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <limits>
using namespace std;

// Person Class
class Person {
private:
    string Name;
    int age;
    string ID;
    string contact;

public:
    Person(string name, int age, string id, string contact) {
        setName(name);
        setAge(age);
        setID(id);
        setContact(contact);
    }

    virtual ~Person() {}

    // Getters
    string getName() const { return Name; }
    int getAge() const { return age; }
    string getID() const { return ID; }
    string getContact() const { return contact; }

    // Setters with validation
    void setName(string name) {
        if (!name.empty())
            Name = name;
        else
            cout << "Error: Name cannot be empty.\n";
    }

    void setAge(int age) {
        if (age > 0 && age <= 130)
            this->age = age;
        else
            cout << "Error: Age must be between 1 and 130.\n";
    }

    void setID(string id) { ID = id; }

    void setContact(string contact) { this->contact = contact; }

    // Virtual method to be overridden by subclasses
    virtual void displayDetails() const {
        cout << "Person Name: " << Name << ", Age: " << age << ", ID: " << ID << ", Contact: " << contact << endl;
    }

    // Virtual method to calculate payment
    virtual float calculatePayment() const {
        return 0.0; // Default implementation for Person
    }
};

// Student Class
class Student : public Person {
private:
    string enrollment_date;
    string program;
    float GPA;

public:
    Student(string name, int age, string id, string contact, string enrollment_date, string program, float GPA)
        : Person(name, age, id, contact), enrollment_date(enrollment_date), program(program), GPA(GPA) {}

    ~Student() {}

    string getEnrollmentDate() const { return enrollment_date; }
    string getProgram() const { return program; }
    float getGPA() const { return GPA; }

    void displayDetails() const override {
        Person::displayDetails();
        cout << "Enrollment Date: " << enrollment_date << ", Program: " << program << ", GPA: " << GPA << endl;
    }

    float calculatePayment() const override {
        return 1000.0; // Example payment for a student
    }
};

// Professor Class
class Professor : public Person {
private:
    string department;
    string specialization;
    string hire_date;

public:
    Professor(string name, int age, string id, string contact, string department, string specialization, string hire_date)
        : Person(name, age, id, contact), department(department), specialization(specialization), hire_date(hire_date) {}

    ~Professor() {}

    string getDepartment() const { return department; }
    string getSpecialization() const { return specialization; }
    string getHireDate() const { return hire_date; }

    void displayDetails() const override {
        Person::displayDetails();
        cout << "Department: " << department << ", Specialization: " << specialization << ", Hire Date: " << hire_date << endl;
    }

    float calculatePayment() const override {
        return 5000.0; // Example payment for a professor
    }
};

// GradeBook Class
class GradeBook {
private:
    map<string, float> grades; // studentID -> grade

public:
    GradeBook() {}

    void addGrade(string studentID, float grade) {
        if (grade >= 0.0 && grade <= 100.0) {
            grades[studentID] = grade;
            cout << "Grade added for student " << studentID << endl;
        } else {
            cout << "Error: Grade must be between 0 and 100." << endl;
        }
    }

    void displayGrades() const {
        cout << "\nGradeBook Entries:" << endl;
        for (auto& entry : grades) {
            cout << "Student ID: " << entry.first << ", Grade: " << entry.second << endl;
        }
    }

    float calculateAverageGrade() const {
        if (grades.empty()) return 0.0;
        float sum = 0.0;
        for (auto& entry : grades) {
            sum += entry.second;
        }
        return sum / grades.size();
    }

    float getHighestGrade() const {
        if (grades.empty()) return 0.0;
        float highest = numeric_limits<float>::min();
        for (auto& entry : grades) {
            if (entry.second > highest)
                highest = entry.second;
        }
        return highest;
    }

    vector<string> getFailingStudents(float passMark = 40.0) const {
        vector<string> failing;
        for (auto& entry : grades) {
            if (entry.second < passMark)
                failing.push_back(entry.first);
        }
        return failing;
    }
};

// EnrollmentManager Class
class EnrollmentManager {
private:
    map<string, vector<string>> courseEnrollments; // courseCode -> list of studentIDs

public:
    EnrollmentManager() {}

    void enrollStudent(string courseCode, string studentID) {
        courseEnrollments[courseCode].push_back(studentID);
        cout << "Student " << studentID << " enrolled in course " << courseCode << endl;
    }

    void dropStudent(string courseCode, string studentID) {
        auto& students = courseEnrollments[courseCode];
        for (auto it = students.begin(); it != students.end(); ++it) {
            if (*it == studentID) {
                students.erase(it);
                cout << "Student " << studentID << " dropped from course " << courseCode << endl;
                return;
            }
        }
        cout << "Student " << studentID << " not found in course " << courseCode << endl;
    }

    int getEnrollmentCount(string courseCode) const {
        if (courseEnrollments.find(courseCode) != courseEnrollments.end()) {
            return courseEnrollments.at(courseCode).size();
        }
        return 0;
    }

    void displayEnrollments() const {
        cout << "\nEnrollment List:" << endl;
        for (auto& course : courseEnrollments) {
            cout << "Course " << course.first << " has students: ";
            for (const string& id : course.second) {
                cout << id << " ";
            }
            cout << endl;
        }
    }
};

// Test function to demonstrate polymorphism
void testPayment(Person* person) {
    cout << "Payment for " << person->getName() << ": " << person->calculatePayment() << endl;
}

int main() {
    // Create an array of Person pointers containing both Student and Professor objects
    Person* people[3];
    people[0] = new Student("Alice", 20, "S1234", "9876543210", "2021-08-15", "Computer Science", 3.8);
    people[1] = new Professor("John", 45, "P9876", "9123456789", "Computer Science", "AI & ML", "2015-06-10");
    people[2] = new Student("Bob", 22, "S5678", "9876543211", "2020-09-10", "Mathematics", 3.5);

    // Iterate through the array and demonstrate polymorphism
    for (int i = 0; i < 3; ++i) {
        cout << "\nDetails of person " << i + 1 << ":" << endl;
        people[i]->displayDetails(); // Calls the overridden displayDetails method
        testPayment(people[i]);      // Calls the overridden calculatePayment method
    }

    // Clean up dynamically allocated memory
    for (int i = 0; i < 3; ++i) {
        delete people[i];
    }

    return 0;
}