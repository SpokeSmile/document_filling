#include "studentitemwidget.h"
//функция реализует конструктор класса StudentItemWidget, предназначенного для представления отдельного студента в графическом интерфейсе
StudentItemWidget::StudentItemWidget(QString studentName, QWidget *parent)
    : QWidget(parent)
{
    m_label = new QLabel(studentName, this);//Виджет, отображающий имя студента
    m_checkbox = new QCheckBox(this);
//Макет:Имя студента слева.Растягивающийся пробел посередине.чекбокс справа.
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_label);
    layout->addStretch();
    layout->addWidget(m_checkbox);

    setLayout(layout);
//сигнал stateChanged от чекбокса к лямбде-функции, которая генерирует собственный сигнал stateChanged(bool) при изменении состояния чекбокса.
     connect(m_checkbox, &QCheckBox::checkStateChanged, this, &StudentItemWidget::onCheckboxStateChanged);
}
void StudentItemWidget::onCheckboxStateChanged(int state)
{
    emit selectionChanged(state == Qt::Checked);
}
