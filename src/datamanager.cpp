#include <iostream>
#include <pqxx/pqxx>
#include "datamanager.h"
#include "structs.h"
#include "mainwindow.h"


QList<Group> syncDatabase() {
    
    QVector<Student> stud;
    QList<Group> gr;



    try {
        pqxx::connection dataBase("dbname=ITTOP user=postgres password=1 host=localhost port=5432");
        
        if (!dataBase.is_open()) {
            std::cerr << "Не удалось подключиться к базе данных\n";
            throw "Не удалось";
        }

        pqxx::work txn(dataBase);

       QVector<Group> gr;

        pqxx::result groups = txn.exec(
            "SELECT DISTINCT group_name FROM \"Students\" ORDER BY group_name;"
        );

        for (const pqxx::row& row : groups) {
            QString group_name = row["group_name"].c_str();

            // Создаём группу
            Group group(group_name);

            // Получаем студентов этой группы
            pqxx::result students_request = txn.exec(
                pqxx::zview{
                    "SELECT last_name, first_name, middle_name FROM \"Students\" WHERE group_name = $1 ORDER BY last_name, first_name;"
                },
                pqxx::params{group_name.toStdString()}
            );

            for (const pqxx::row& student_row : students_request) {
                Student s(
                    student_row["last_name"].c_str(),
                    student_row["first_name"].c_str(),
                    student_row["middle_name"].c_str()
                );

                group.setStudents(s);
            }

            gr.append(group);
        }
        

        txn.commit();
    } catch (const std::exception &e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
    
    return gr;
}
