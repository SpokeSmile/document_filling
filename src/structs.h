// #ifndef STRUCTS_H
// #define STRUCTS_H

// #include <qvector.h>
// #include <QMap>


// struct Student
// {
// private:
//     QString last_name;
//     QString first_name;
//     QString middle_name;
//     QString group_name;
//     bool selected;
//     // QMap<QString, unsigned short> marks;

// public:
//     Student() = default;
//     Student(const QString& last, const QString& first, const QString& middle, const QString& group) 
//         : last_name(last), first_name(first), middle_name(middle), group_name(group), selected(false){}

//     QString getFullName() const { return last_name + " " + first_name + " " + middle_name; }
//     QString getGroup() const { return group_name; }
 
 
//     // const QString& getName() const { return name; }
//     // void setName(const QString& newName) { name = newName; }
//     // const QMap<QString, unsigned short>& getMarks() const { return marks; }
//     // void setMark(const QString& subject, unsigned short mark) { marks.insert(subject, mark); }

//     bool isSelected() const { return selected; }
//     void setSelected(bool value) { selected = value; }
// };

// #endif // STRUCTS_H
#ifndef STRUCTS_H
#define STRUCTS_H

#include <qvector.h>
#include <QMap>


struct Students
{
private:
    QString name;
    bool selected;
    QString ticketNumber;
    QString formOfStudy;

public:
    Students() = default;
    Students(const QString& n): name(n){};

    const QString& getName() const { return name; }
    void setName(const QString& newName) { name = newName; }




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
    QVector<Students> StudList;


public:
    Group() = default;


    Group(const QString& groupName): name(groupName) { }


    void addStudent(const Students& student) {
        StudList.push_back(student);
    }


    const QVector<Students>& getStudents() const { return StudList; }


    const QString& getName() const { return name; }
    void setName(const QString& newName) { name = newName; }
};

#endif // STRUCTS_H
