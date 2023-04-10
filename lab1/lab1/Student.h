#ifndef STUDENT_H_
#define STUDENT_H_

#include <string>
#include <vector>

class Class;

enum Degree {
    undergraduate,
    graduate
};

class Student {
    // (Done)TODO: implement class Student.
private:
    std::string name, year;
    Degree degree;
protected:
    std::vector<Class *> classes;
public:
    const std::string id;
    Student(const std::string &id, const std::string &name, const std::string &year, const Degree degree)
        : name(name), year(year), degree(degree), id(id) { }
    virtual ~Student() { }
    virtual double getGrade() = 0;
    std::string toString() const;
    void addClass(Class* c)
    {
        classes.push_back(c);
    }
};

// (Done)TODO: implement class Graduate.
class Graduate: public Student {
public:
    Graduate(const std::string &id, const std::string &name, const std::string &year)
        : Student(id, name, year, graduate) { }
    virtual double getGrade();
};

// (Done)TODO: implement class Undergraduate.
class Undergraduate: public Student {
public:
    Undergraduate(const std::string &id, const std::string &name, const std::string &year)
        : Student(id, name, year, undergraduate) { }
    virtual double getGrade();
};

class StudentWrapper {
private:
    const Student &student;
    double score = 0.0;
public:
    const std::string id;
    // (Done)TODO: fix error
    StudentWrapper(const std::string &id, const Student &student)
        : student(student), id(id) { }

    void setScore(double score)
    {
        if (score < 0 || score > 100)
            throw "Wrong score!";
        this->score = score;
    }

    double getScore() const
    {
        return this->score;
    }

    std::string toString() const
    {
        return student.toString();
    }
};

#endif // STUDENT_H_
