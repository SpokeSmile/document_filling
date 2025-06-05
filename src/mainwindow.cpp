#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "structs.h"
#include "studentitemwidget.h"
#include <QListWidget>
#include <QCheckBox>
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->StudentsList->setSelectionMode(QAbstractItemView::NoSelection);

    groups = generateGroup();
//заполняем группы в QComboBox
    for(auto el : groups) {
        ui->SearchBox->addItem(el.getName());
    }
//сигналы (обработчики кнопок первый аргумент название интерфейса второй тип сигнала третий к чему применяется четвертое какую функцию применить)
    connect(ui->selectAllButton, &QPushButton::clicked, this, &MainWindow::onSelectAllClicked);
    connect(ui->SearchBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSearchBoxCurrentIndexChanged(int)));
    connect(ui->generatePdfButton, &QPushButton::clicked, this, &MainWindow::onGeneratePdfButtonClicked);

}
//заполняем группы в QComboBox
QList<Group> MainWindow::generateGroup()
{
    QList<Group> result;

    Group RPO1("РПО 24/1 11/1");
    Students max("Трудков Максим Сергеевич");
    max.setTicketNumber("B12345");
    max.setFormOfStudy("Очная");
    RPO1.addStudent(max);

    Students pavel("Шуткин Павел Анатольевич");
    pavel.setTicketNumber("B12346");
    pavel.setFormOfStudy("Очная");
    RPO1.addStudent(pavel);
    result.append(RPO1);

    Group RPO2("РПО 24/2 11/2");
    Students ten("Тен Максим ЗабылОтчество");
    ten.setTicketNumber("B12349");
    ten.setFormOfStudy("Очная");
    RPO2.addStudent(ten);
    result.append(RPO2);

    return result;
}

void MainWindow::showStudents(const Group& currentGroup)
{
    ui->StudentsList->clear();//чистим

    for(const Students& student : currentGroup.getStudents())
    {
        //создаем новый элемент интерфейса дял каждого студента
        StudentItemWidget *widget = new StudentItemWidget(student.getName(), ui->StudentsList);
        QListWidgetItem *item = new QListWidgetItem(ui->StudentsList);
        item->setSizeHint(widget->minimumSizeHint());
        ui->StudentsList->setItemWidget(item, widget);
    }
}
//отображение группы в QComboBox
void MainWindow::onSearchBoxCurrentIndexChanged(int index)
{
    if (index >= 0 && index < groups.size()) {
        const Group& selectedGroup = groups[index];
        showStudents(selectedGroup);
    }
}

void MainWindow::onSelectAllClicked()
{
    int currentIndex = ui->SearchBox->currentIndex();
    if (currentIndex >= 0 && currentIndex < groups.size()) {
        auto group = groups[currentIndex];
        auto students = group.getStudents();

        bool allUnchecked = true;

        for(int i = 0; i < ui->StudentsList->count(); i++) {
            auto widget = dynamic_cast<StudentItemWidget*>(ui->StudentsList->itemWidget(ui->StudentsList->item(i)));

            if (!widget || !(widget->findChild<QCheckBox*>())) continue;

            QCheckBox* checkbox = widget->findChild<QCheckBox*>();
            bool isChecked = checkbox->isChecked();

            if(isChecked) {
                allUnchecked = false;
            }

            bool newState = !allUnchecked;
            checkbox->setChecked(newState);
            students[i].setSelected(newState);
        }
    }
}
//переписать функцию обработки комбо боксов и кнопки выбраить всех
/*void MainWindow::onSelectAllClicked()
{
    bool anySelected = false;
    int currentIndex = ui->SearchBox->currentIndex();
    if (currentIndex >= 0 && currentIndex < groups.size()) {
        auto group = groups[currentIndex];
        auto students = group.getStudents();

        // Проверяем наличие выделенных элементов
        for(int i = 0; i < ui->StudentsList->count(); ++i) {
            auto widget = dynamic_cast<StudentItemWidget*>(ui->StudentsList->itemWidget(ui->StudentsList->item(i)));
            if (widget != nullptr && widget->findChild<QCheckBox*>()->isChecked()) {
                anySelected = true;
                break;
            }
        }

        // Переключаем состояние
        bool selectState = !anySelected;
        for(int i = 0; i < ui->StudentsList->count(); ++i) {
            auto widget = dynamic_cast<StudentItemWidget*>(ui->StudentsList->itemWidget(ui->StudentsList->item(i)));
            if (widget != nullptr) {
                widget->findChild<QCheckBox*>()->setChecked(selectState);
                students[i].setSelected(selectState); // Меняем состояние в данных
            }
        }
    }
}
*/


//вектор выделенных студентов который надо передать дял генарции пдф
std::vector<Students> MainWindow::getSelectedStudents(const Group& group)
{
    std::vector<Students> result;
    for(const auto& student : group.getStudents()) {
        if(student.isSelected()) {
            result.push_back(student);

        }
    }
    return result;
}
//генерация пдф
void MainWindow::onGeneratePdfButtonClicked()
{
    // Получаем индекс текущего выбранного группы
    int currentIndex = ui->SearchBox->currentIndex();
    if (currentIndex >= 0 && currentIndex < groups.size()) {
        const Group& currentGroup = groups[currentIndex]; // Текущая группа
        std::vector<Students> selectedStudents = getSelectedStudents(currentGroup); // Выбранные студенты

        // Запрашиваем имя файла
        QString fileName = QFileDialog::getSaveFileName(
            this,tr("Сохранить отчет"),"",tr("Документы PDF (*.pdf)"));

        if (!fileName.isEmpty()) {

            m_pdfGenerator->generatePdf(fileName, selectedStudents);
        }
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}
