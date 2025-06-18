#include <iostream>
#include <cstring>
using namespace std;

// Structure to represent a module
struct Module {
    char code[8];
    char title[30];
    int credits;
};

// Abstract base class representing a person
class Person {
protected:
    char id[10];
    char name[30];
public:
    Person(const char* pid, const char* pname) {
        strcpy(id, pid);
        strcpy(name, pname);
    }

    virtual void showInfo() = 0;  // Pure virtual function
    const char* getId() const {
        return id;
    }

    virtual ~Person() {}
};

// Class representing a student (inherits from Person)
class Student : public Person {
    Module* enrolledModules;
    int moduleCount;
public:
    Student(const char* id, const char* name)
        : Person(id, name), enrolledModules(nullptr), moduleCount(0) {}

    void addModule(const Module& mod) {
        Module* newArray = new Module[moduleCount + 1];
        for (int i = 0; i < moduleCount; ++i)
            *(newArray + i) = *(enrolledModules + i);
        *(newArray + moduleCount) = mod;
        delete[] enrolledModules;
        enrolledModules = newArray;
        ++moduleCount;
    }

    void showInfo() override {
        cout << "[Student] ID: " << id << ", Name: " << name << "\nEnrolled Modules:\n";
        for (int i = 0; i < moduleCount; ++i) {
            Module* mod = enrolledModules + i;
            cout << "- " << mod->code << " (" << mod->title << "), " << mod->credits << " credits\n";
        }
        cout << endl;
    }

    ~Student() {
        delete[] enrolledModules;
    }
};

// Class representing an instructor (inherits from Person)
class Instructor : public Person {
    Module* taughtModules;
    int moduleCount;
public:
    Instructor(const char* id, const char* name)
        : Person(id, name), taughtModules(nullptr), moduleCount(0) {}

    void addModule(const Module& mod) {
        Module* newArray = new Module[moduleCount + 1];
        for (int i = 0; i < moduleCount; ++i)
            *(newArray + i) = *(taughtModules + i);
        *(newArray + moduleCount) = mod;
        delete[] taughtModules;
        taughtModules = newArray;
        ++moduleCount;
    }

    void showInfo() override {
        cout << "[Instructor] ID: " << id << ", Name: " << name << "\nTaught Modules:\n";
        for (int i = 0; i < moduleCount; ++i) {
            Module* mod = taughtModules + i;
            cout << "- " << mod->code << " (" << mod->title << "), " << mod->credits << " credits\n";
        }
        cout << endl;
    }

    ~Instructor() {
        delete[] taughtModules;
    }
};

// Class managing people (students + instructors)
class University {
    Person** people;
    int count;
public:
    University() : people(nullptr), count(0) {}

    // Add a new Person (Student or Instructor)
    void addPerson(Person* p) {
        Person** newArray = new Person*[count + 1];
        for (int i = 0; i < count; ++i)
            *(newArray + i) = *(people + i);
        *(newArray + count) = p;
        delete[] people;
        people = newArray;
        ++count;
    }

    // Remove a Person by ID
    void removePerson(const char* id) {
        int index = -1;
        for (int i = 0; i < count; ++i) {
            if (strcmp(people[i]->getId(), id) == 0) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            cout << "Person not found.\n";
            return;
        }

        delete people[index];

        Person** newArray = new Person*[count - 1];
        for (int i = 0, j = 0; i < count; ++i) {
            if (i != index)
                *(newArray + j++) = *(people + i);
        }

        delete[] people;
        people = newArray;
        --count;
    }

    // Display all people and their modules
    void showAllPeople() {
        for (int i = 0; i < count; ++i) {
            people[i]->showInfo();
        }
    }

    ~University() {
        for (int i = 0; i < count; ++i)
            delete people[i];
        delete[] people;
    }
};

// Main function to test the system
int main() {
    University u;

    // Sample modules
    Module m1 = {"BIT1001", "Programming I", 10};
    Module m2 = {"BIT1002", "OOP Concepts", 12};
    Module m3 = {"BIT2001", "Data Structures", 15};

    // Create and add a student
    Student* s1 = new Student("S001", "Alice");
    s1->addModule(m1);
    s1->addModule(m2);

    // Create and add an instructor
    Instructor* i1 = new Instructor("I001", "Dr. John");
    i1->addModule(m2);
    i1->addModule(m3);

    u.addPerson(s1);
    u.addPerson(i1);

    cout << "=== All People in University ===\n";
    u.showAllPeople();

    cout << "=== Removing Student S001 ===\n";
    u.removePerson("S001");

    cout << "=== After Removal ===\n";
    u.showAllPeople();

    return 0;
}

