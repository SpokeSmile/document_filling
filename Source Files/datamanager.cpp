#include <iostream>
#include <pqxx/pqxx>
#include "datamanager.h"
#include "structs.h"
#include "mainwindow.h"


QList<Group> loadGroupsFromDatabase() {
    
    QVector<Student> stud;
    QList<Group> gr;



    try {
        pqxx::connection dataBase("dbname=ITTOP user=postgres password=1 host=localhost port=5432");
        
        if (!dataBase.is_open()) {
            std::cerr << "Не удалось подключиться к базе данных\n";
            throw "Не удалось";
        }

        pqxx::work txn(dataBase);


        pqxx::result groups = txn.exec(
            "SELECT DISTINCT group_name FROM \"Students\" ORDER BY group_name;"
        );

        for (const pqxx::row& row : groups) {
            QString group_name = row["group_name"].c_str();

            // Создаём группу
            Group group(group_name);

            // Получаем студентов этой группы
            pqxx::result students_request = txn.exec(
                pqxx::zview{R"(
                    SELECT 
                        s.last_name,
                        s.first_name,
                        s.middle_name,
                        s.group_name,
                        t.ticket_number,
                        t.education_form,
                        t.order_date,
                        t.order_number,
                        t.issue_date,
                        t.valid_until
                    FROM "Students" s
                    JOIN "StudentTicket" t ON s.id = t.students_id
                    WHERE s.group_name = $1
                    ORDER BY s.last_name, s.first_name;
                )"},
                pqxx::params{group_name.toStdString()}
            );

            // Добовляем студентов в QList<Group>
            for (const pqxx::row& student_row : students_request) {
                Student s(
                    student_row["last_name"].c_str(),
                    student_row["first_name"].c_str(),
                    student_row["middle_name"].is_null() ? "" : student_row["middle_name"].c_str(),
                    student_row["ticket_number"].c_str(),
                    student_row["education_form"].c_str()
                );

                group.addStudent(s);
            }

            gr.append(group);
        }
        
        txn.commit();
    } catch (const std::exception &e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
    
    
    return gr;
}
