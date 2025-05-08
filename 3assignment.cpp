#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <limits>
using namespace std;

class Person 
{
protected:
    string name, id, contact;
    int age;

public:
    Person(string name, int age, string id, string contact)
        : name(name), age(age), id(id), contact(contact) {}
    virtual ~Person() {}

    virtual void displayDetails() const 
    {
        cout << "Name: " << name << ", Age: " << age << ", ID: " << id << ", Contact: " << contact << endl;
    }

    virtual float calculatePayment() const 
    {
         return 0.0; 
    }
};

class Student : public Person 
{
protected:
    string enrollment_date, program;
    float GPA;
    vector<class Course*> courses;

public:
    Student(string name, int age, string id, string contact, string enrollment_date, string program, float GPA)
        : Person(name, age, id, contact), enrollment_date(enrollment_date), program(program), GPA(GPA) {}

    void enrollInCourse(class Course* course) 
    {
         courses.push_back(course); 
    }

    void displayDetails() const override 
    {
        Person::displayDetails();
        cout << "Enrollment: " << enrollment_date << ", Program: " << program << ", GPA: " << GPA << endl;
    }

    float calculatePayment() const override 
    {
         return 1000.0; 
    }
};

class UndergraduateStudent : public Student 
{
    string major, minor, graduation_date;
public:
    UndergraduateStudent(string name, int age, string id, string contact, string enrollment_date, string program, float GPA,
                         string major, string minor, string graduation_date)
        : Student(name, age, id, contact, enrollment_date, program, GPA), major(major), minor(minor), graduation_date(graduation_date) {}

    void displayDetails() const override 
    {
        Student::displayDetails();
        cout << "Major: " << major << ", Minor: " << minor << ", Graduation: " << graduation_date << endl;
    }
};

class GraduateStudent : public Student 
{
    string research_topic, advisor, thesis_title;
    int teaching_assist_hours = 0;
    int research_assist_hours = 0;
public:
    GraduateStudent(string name, int age, string id, string contact, string enrollment_date, string program, float GPA,
                    string research_topic, string advisor, string thesis_title)
        : Student(name, age, id, contact, enrollment_date, program, GPA), research_topic(research_topic), advisor(advisor), thesis_title(thesis_title) {}

    void displayDetails() const override 
    {
        Student::displayDetails();
        cout << "Research Topic: " << research_topic << ", Advisor: " << advisor << ", Thesis: " << thesis_title << endl;
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
    Professor(string name, int age, string id, string contact, string department,
              string specialization, string hire_date, int years, float base_salary, float grants)
        : Person(name, age, id, contact), department(department), specialization(specialization),
          hire_date(hire_date), years_of_service(years), base_salary(base_salary), research_grants(grants) {}

    void displayDetails() const override 
    {
        Person::displayDetails();
        cout << "Dept: " << department << ", Spec: " << specialization << ", Hire Date: " << hire_date << endl;
    }
};

class AssistantProfessor : public Professor 
{
public:
    AssistantProfessor(string name, int age, string id, string contact, string department,
                       string specialization, string hire_date, int years, float salary, float grants)
        : Professor(name, age, id, contact, department, specialization, hire_date, years, salary, grants) {}

    float calculatePayment() const override 
    {
        return base_salary + years_of_service * 300 + research_grants;
    }
};

class AssociateProfessor : public Professor 
{
public:
    AssociateProfessor(string name, int age, string id, string contact, string department,
                       string specialization, string hire_date, int years, float salary, float grants)
        : Professor(name, age, id, contact, department, specialization, hire_date, years, salary, grants) {}

    float calculatePayment() const override 
    {
        return base_salary + years_of_service * 500 + research_grants * 1.2;
    }
};

class FullProfessor : public Professor 
{
public:
    FullProfessor(string name, int age, string id, string contact, string department,
                  string specialization, string hire_date, int years, float salary, float grants)
        : Professor(name, age, id, contact, department, specialization, hire_date, years, salary, grants) {}

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
    Course(string code, Professor* prof) : courseCode(code), instructor(prof) {}
    string getCode() const 
    {
         return courseCode; 
    }
    Professor* getInstructor() const 
    {
         return instructor; 
    }
};

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
        for (auto& entry : timetable) 
        {
            cout << "Course: " << entry.first->getCode() << ", Time: " << entry.second.first
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
    GraduateStudent grad("Bob", 27, "GS001", "9876543210", "2021-08-15", "CS", 3.9, "AI", "Dr. Smith", "AI Optimization");
    grad.addTeachingHours(10);
    grad.addResearchHours(15);
    grad.displayDetails();
    cout << "Grad Payment: $" << grad.calculatePayment() << endl;

    FullProfessor prof("Dr. Jane", 55, "P001", "9123456789", "CS", "AI", "2010-05-01", 15, 6000, 10000);
    prof.displayDetails();
    cout << "Prof Payment: $" << prof.calculatePayment() << endl;

    return 0;
}