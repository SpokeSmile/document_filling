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
    QString ticketNumber;
    QString educationForm;
    QString orderDate;
    QString orderNumber;
    QString issueDate;
    QString validUntil;
    bool selected = false;
    
public:
    Student() = default;
    Student(const QString& last, const QString& first, const QString& middle, const QString& ticketNum, const QString& educationForm, const QString& orderDate, const QString& orderNumber, const QString& issueDate, const QString& validUntil)
        : last_name(last), first_name(first), middle_name(middle), ticketNumber(ticketNum), educationForm(educationForm), orderDate(orderDate), orderNumber(orderNumber), issueDate(issueDate), validUntil(validUntil)  {};

    const QString& getLastName() const { return last_name; }
    const QString& getFirstName() const { return first_name; }
    const QString& getMiddleName() const { return middle_name; }
    
    QString getFullName() const {
        return last_name + " " + first_name + " " + middle_name;
    }


    bool isSelected() const { return selected; }
    void setSelected(bool selectState) { selected = selectState; }

    QString getTicketNumber() const { return ticketNumber; }
    QString getFormOfStudy() const { return educationForm; }
    QString getOrderDate() const { return orderDate; }
    QString getOrderNumber() const { return orderNumber; }
    QString getIssueDate() const { return issueDate; }
    QString getValidUntil() const { return validUntil; }
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
