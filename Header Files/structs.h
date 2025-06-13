#ifndef STRUCTS_H
#define STRUCTS_H

#include <qvector.h>
#include <QMap>


struct Student
{
private:
    QString last_name;
    QString first_name;
    QString middle_name;
    bool selected = false;
    QString ticketNumber;
    QString formOfStudy;

public:
    Student() = default;
    Student(const QString& last, const QString& first, const QString& middle, const QString& ticketNum, const QString& formOfStudy)
        : last_name(last), first_name(first), middle_name(middle), ticketNumber(ticketNum), formOfStudy(formOfStudy)  {};

    const QString& getLastName() const { return last_name; }
    const QString& getFirstName() const { return first_name; }
    const QString& getMiddleName() const { return middle_name; }
    
    QString getFullName() const {
        return last_name + " " + first_name + " " + middle_name;
    }





    bool isSelected() const { return selected; }
    void setSelected(bool value) { selected = value; }

    void setTicketNumber(const QString& number) { ticketNumber = number; }
    QString getTicketNumber() const { return ticketNumber; }

    void setFormOfStudy(const QString& form) { formOfStudy = form; }
    QString getFormOfStudy() const { return formOfStudy; }
};
//----------------

struct Group
{
private:
    QString name;
    QVector<Student> StudList;


public:
    Group() = default;

    Group(const QString& groupName): name(groupName) { }


    void addStudent(const Student& student) {
        StudList.push_back(student);
    }


    QVector<Student>& getStudents() { return StudList; }
    

    const QString& getName() const { return name; }
    void setName(const QString& newName) { name = newName; }
};

#endif // STRUCTS_H
