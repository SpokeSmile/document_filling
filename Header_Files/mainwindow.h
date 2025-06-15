#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "structs.h"
#include <QMainWindow>
#include <QList.h>
#include "pdfgenerator.h"
#include "datamanager.h"
//обьясвляем функции связаные с отображением интерфейса и сигналов
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QList<Group> generateGroup();
    //слоты для сигналов
public slots:
    void onGeneratePdfButtonClicked();
private slots:
    void onSearchBoxCurrentIndexChanged(int index);
    void onSelectAllClicked();

private:
    Ui::MainWindow *ui;
    QList<Group> groups = loadGroupsFromDatabase();
    
    void showStudents(Group& currentGroup);

    //получаем выбранных студентов по заданной группе
    std::vector<Student> getSelectedStudents(Group& group);

    std::unique_ptr<PdfGenerator> m_pdfGenerator;//экземлпяр qpdfgenerator
};

#endif // MAINWINDOW_H
