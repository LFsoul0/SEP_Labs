#include "Class.h"
#include <string>
#include "Student.h"

void Class::addStudent(const Student &st)
{
    StudentWrapper sw(st.id, st);
    students.push_back(sw);
}

StudentWrapper &Class::getStudentWrapper(const std::string &studentId)
{
    for (std::vector<StudentWrapper>::iterator it = students.begin();
         it != students.end();
         ++it) {
        if (it->id == studentId)
            return *it;
    }
    throw "No match student!";
}

double Class::getAvgScore()
{
    // (Done)TODO: implement getAvgScore.
    int count = 0;
    double AvgScore = 0;

    for (std::vector<StudentWrapper>::iterator it = students.begin();
         it != students.end();
         ++it) {
        AvgScore += it->getScore();
        count++;
    }
    AvgScore /= count;

    return AvgScore;

    //return 0.0;
}
