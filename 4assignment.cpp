#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <limits>
#include <exception>
#include <sstream>
#include <fstream>
using namespace std;

void logException(const string& message)
{
    ofstream logFile("university_exceptions.log", ios::app);
    if (logFile.is_open()) 
    {
        logFile << message << endl;
        logFile.close();
    }
}

class UniversitySystemException : public exception 
{
protected:
    string message;
public:
    UniversitySystemException(const string& msg) : message(msg) 
    {
        logException(msg);
    }

    virtual const char* what() const noexcept override 
    {
        return message.c_str();
    }
};

class EnrollmentException : public UniversitySystemException 
{
    string studentID;
    string courseCode;
public:
    EnrollmentException(const string& studentID, const string& courseCode, const string& msg)
        : UniversitySystemException(msg), studentID(studentID), courseCode(courseCode) 
    {
        ostringstream oss;
        oss << "EnrollmentException for Student ID: " << studentID
            << ", Course: " << courseCode << " - " << msg;
        message = oss.str();
        logException(message);
    }
};

class GradeException : public UniversitySystemException 
{
    string studentID;
    float invalidGrade;
public:
    GradeException(const string& studentID, float grade, const string& msg)
        : UniversitySystemException(msg), studentID(studentID), invalidGrade(grade) 
    {
        ostringstream oss;
        oss << "GradeException for Student ID: " << studentID
            << ", Invalid Grade: " << grade << " - " << msg;
        message = oss.str();
        logException(message);
    }
};

class PaymentException : public UniversitySystemException 
{
    string personID;
    float amount;
public:
    PaymentException(const string& personID, float amount, const string& msg)
        : UniversitySystemException(msg), personID(personID), amount(amount)
    {
        ostringstream oss;
        oss << "PaymentException for ID: " << personID
            << ", Amount: $" << amount << " - " << msg;
        message = oss.str();
        logException(message);
    }
};


class Person 
{
protected:
    string name, id, contact;
    int age;
public:
    Person(string name, int age, string id, string contact)
    : name(name), age(age), id(id), contact(contact) 
    {
        if (id.empty() || contact.length() < 10) 
        {
            throw UniversitySystemException("Invalid ID or contact information for person: " + name);
        }
    }

    virtual ~Person() {}

    virtual void displayDetails() const 
    {
        cout << "Name: " << name << ", Age: " << age
             << ", ID: " << id << ", Contact: " << contact << endl;
    }

    virtual float calculatePayment() const 
    {
        throw PaymentException(id, 0.0, "Payment calculation not defined for base class Person.");
    }

    string getID() const 
    {
         return id; 
    }
};

class Course; 
class Student : public Person 
{
protected:
    string enrollment_date, program;
    float GPA;
    vector<Course*> courses;

public:
    Student(string name, int age, string id, string contact,
            string enrollment_date, string program, float GPA)
        : Person(name, age, id, contact),
          enrollment_date(enrollment_date), program(program), GPA(GPA) 
        {
        if (GPA < 0.0 || GPA > 4.0) 
        {
            throw GradeException(id, GPA, "Invalid GPA. Must be between 0.0 and 4.0");
        }
    }

    void enrollInCourse(Course* course);

    void displayDetails() const override 
    {
        Person::displayDetails();
        cout << "Enrollment: " << enrollment_date
             << ", Program: " << program << ", GPA: " << GPA << endl;
    }

    float calculatePayment() const override 
    {
        return 1000.0; // base tuition fee
    }
};

class UndergraduateStudent : public Student 
{
    string major, minor, graduation_date;
public:
    UndergraduateStudent(string name, int age, string id, string contact,
                         string enrollment_date, string program, float GPA,
                         string major, string minor, string graduation_date)
        : Student(name, age, id, contact, enrollment_date, program, GPA),
          major(major), minor(minor), graduation_date(graduation_date) {}

    void displayDetails() const override 
    {
        Student::displayDetails();
        cout << "Major: " << major << ", Minor: " << minor
             << ", Graduation: " << graduation_date << endl;
    }
};

class GraduateStudent : public Student 
{
    string research_topic, advisor, thesis_title;
    int teaching_assist_hours = 0;
    int research_assist_hours = 0;
public:
    GraduateStudent(string name, int age, string id, string contact,
                    string enrollment_date, string program, float GPA,
                    string research_topic, string advisor, string thesis_title)
        : Student(name, age, id, contact, enrollment_date, program, GPA),
          research_topic(research_topic), advisor(advisor), thesis_title(thesis_title) {}

    void displayDetails() const override 
    {
        Student::displayDetails();
        cout << "Research Topic: " << research_topic
             << ", Advisor: " << advisor
             << ", Thesis: " << thesis_title << endl;
    }

    void addTeachingHours(int hours) 
    {
        teaching_assist_hours += hours; 
    }
    void addResearchHours(int hours) 
    {
         research_assist_hours += hours; 
    }

    float calculatePayment() const override 
    {
        return 1500.0 + teaching_assist_hours * 20 + research_assist_hours * 25;
    }
};

class Professor : public Person 
{
protected:
    string department, specialization, hire_date;
    int years_of_service;
    float base_salary;
    float research_grants;

public:
    Professor(string name, int age, string id, string contact,
              string department, string specialization, string hire_date,
              int years, float base_salary, float grants)
        : Person(name, age, id, contact), department(department), specialization(specialization),
          hire_date(hire_date), years_of_service(years), base_salary(base_salary), research_grants(grants) {}

    void displayDetails() const override 
    {
        Person::displayDetails();
        cout << "Dept: " << department << ", Spec: " << specialization
             << ", Hire Date: " << hire_date << endl;
    }
};

class AssistantProfessor : public Professor 
{
public:
    AssistantProfessor(string name, int age, string id, string contact,
                       string department, string specialization, string hire_date,
                       int years, float salary, float grants)
        : Professor(name, age, id, contact, department, specialization,
                    hire_date, years, salary, grants) {}

    float calculatePayment() const override 
    {
        return base_salary + years_of_service * 300 + research_grants;
    }
};

class AssociateProfessor : public Professor 
{
public:
    AssociateProfessor(string name, int age, string id, string contact,
                       string department, string specialization, string hire_date,
                       int years, float salary, float grants)
        : Professor(name, age, id, contact, department, specialization,
                    hire_date, years, salary, grants) {}

    float calculatePayment() const override 
    {
        return base_salary + years_of_service * 500 + research_grants * 1.2;
    }
};

class FullProfessor : public Professor 
{
public:
    FullProfessor(string name, int age, string id, string contact,
                  string department, string specialization, string hire_date,
                  int years, float salary, float grants)
        : Professor(name, age, id, contact, department, specialization,
                    hire_date, years, salary, grants) {}

    float calculatePayment() const override 
    {
        return base_salary + years_of_service * 800 + research_grants * 1.5;
    }
};

class Course 
{
    string courseCode;
    Professor* instructor;
public:
    Course(string code, Professor* prof)
        : courseCode(code), instructor(prof) {}

    string getCode() const 
    {
         return courseCode; 
    }
    Professor* getInstructor() const 
    {
         return instructor; 
    }
};

void Student::enrollInCourse(Course* course) 
{
    if (!course) 
    {
        throw EnrollmentException(id, "UNKNOWN", "Cannot enroll in a null course.");
    }
    courses.push_back(course);
}

class Department 
{
    string name;
    vector<Professor*> professors;
public:
    Department(string name) : name(name) {}
    void addProfessor(Professor* prof) 
    {
         professors.push_back(prof); 
    }
    vector<Professor*> getProfessors() const 
    {
         return professors; 
    }
    string getName() const 
    {
         return name; 
    }
};

class Classroom 
{
    string roomNumber;
public:
    Classroom(string room) : roomNumber(room) {}
    string getRoomNumber() const 
    {
         return roomNumber; 
    }
};

class Schedule 
{
    map<Course*, pair<string, Classroom*>> timetable;
public:
    void assignSlot(Course* course, string time, Classroom* room) 
    {
        timetable[course] = {time, room};
    }

    void displaySchedule() const 
    {
        for (const auto& entry : timetable) 
        {
            cout << "Course: " << entry.first->getCode()
                 << ", Time: " << entry.second.first
                 << ", Room: " << entry.second.second->getRoomNumber() << endl;
        }
    }
};

class University 
{
    vector<Department*> departments;
public:
    void addDepartment(Department* dept) 
    {
         departments.push_back(dept); 
    }
    void listDepartments() const 
    {
        for (auto& d : departments)
        {
            cout << d->getName() << endl;
        }
    }
};

int main() 
{
    try 
    {
        GraduateStudent grad("Manjeet", 19, "GS001", "9876543210", "2021-08-15", "CS", 3.9, "AI", "Dr. Krish", "AI Optimization");
        grad.addTeachingHours(10);
        grad.addResearchHours(15);
        grad.displayDetails();
        cout << "Grad Payment: $" << grad.calculatePayment() << endl;

        FullProfessor prof("Mrs Richa Singh", 40, "P001", "9123456789", "CS", "AI", "2010-05-01", 15, 6000, 10000);
        prof.displayDetails();
        cout << "Prof Payment: $" << prof.calculatePayment() << endl;

        Course* nullCourse = nullptr;
        grad.enrollInCourse(nullCourse); // Will trigger EnrollmentException
    }
    catch (const UniversitySystemException& ex) 
    {
        cerr << "Exception: " << ex.what() << endl;
    }

    return 0;
}