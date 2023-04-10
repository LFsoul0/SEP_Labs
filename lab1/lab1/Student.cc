#include "Student.h"
#include <string>
#include <sstream>
#include "Class.h"

std::string Student::toString() const
{
    // (Done)TODO: uncomment the following code after implementing class Student.

    std::ostringstream oss;
    oss << "Student Information:"
        << "\n\tid: " << id
        << "\n\tname: " << name
        << "\n\tenrollment year: " << year
        << "\n\tdegree: " << (degree == graduate ? "graduate" : "undergraduate")
        << std::endl;
    return oss.str();

    //return "";
}

// (Done)TODO: implement functions which are declared in Student.h.
double Undergraduate::getGrade()
{
    int points = 0;
    double gpa = 0;

    for (std::vector<Class *>::iterator it = classes.begin();
         it != classes.end();
         ++it) {
        gpa += (*it)->getStudentWrapper(id).getScore() * (*it)->point / 20;
        points += (*it)->point;
    }
    gpa /= points;

    return gpa;
}

double Graduate::getGrade()
{
    int points = 0;
    double gpa = 0, score;

    for (std::vector<Class *>::iterator it = classes.begin();
         it != classes.end();
         ++it) {
        score = (*it)->getStudentWrapper(id).getScore();

        if (score >= 90) {
            gpa += 4.0 * (*it)->point;
        } else if (score >= 80) {
            gpa += 3.5 * (*it)->point;
        } else if (score >= 70) {
            gpa += 3.0 * (*it)->point;
        } else if (score >= 60) {
            gpa += 2.5 * (*it)->point;
        } else {
            gpa += 2.0 * (*it)->point;
        }

        points += (*it)->point;
    }
    gpa /= points;

    return gpa;
}
