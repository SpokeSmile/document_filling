#include "pdfgenerator.h"
#include <QPdfWriter>
#include <QPainter>
#include <QFontMetrics>
#include <QRectF>
//Инициализация объекта генератора PDF
PdfGenerator::PdfGenerator(QObject *parent) :
    QObject(parent)
{

}

void PdfGenerator::generatePdf(const QString& filename, const std::vector<Student>& students)
{
    QPdfWriter writer(filename);
    writer.setPageSize(QPageSize::A4);
    writer.setResolution(72);
/*
Здесь создается объект QPdfWriter, который ответственен за запись данных в PDF-файл. Заданы следующие настройки:

    Размер страницы формата A4.
    Разрешение экрана — 72 DPI (точек на дюйм)
 */
    QPainter painter(&writer);
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 12));
/*
 * Создается объект QPainter, который будет рисовать текст на PDF. Задаваются черный цвет кисти и шрифт Arial размером 12 пунктов
 */
    QRectF rect(50, 50, 500, 700);
//Определяется прямоугольник, ограничивающий область, в которой будет происходить рисование текста. Координаты (x=50,y=50)(x=50,y=50) задают положение верхнего левого угла прямоугольника, ширина равна 500 пикселей, высота — 700 пикселей.
    int yPosition = 50;
    //Для каждого студента из предоставленного вектора информация извлекается и разбивается на строки для последующего вывода на страницу
    for (const Student& student : students) {
        QString fullName = student.getFullName();
        QStringList nameParts = fullName.split(' ');
        QString surname = nameParts.at(0);
        QString firstName = nameParts.at(1);
        QString patronymic = nameParts.count() > 2 ? nameParts.at(2) : "";

        // Список строк для вывода
        QStringList lines = {
            QString("Студенческий билет №%1").arg(student.getTicketNumber()),
            surname,
            firstName,
            patronymic,
            QString("Форма обучения: %1").arg(student.getFormOfStudy()),
            "Зачислен приказом: 30.08.2024",
            "Дата выдачи: 30 сентября 2024"
        };

        // Рисуем каждую строку отдельно
        for (const QString& line : lines) {
            painter.drawText(rect.x(), yPosition, line);
            yPosition += QFontMetrics(painter.font()).lineSpacing();
        }

        // Небольшое расстояние между студентами
        yPosition += QFontMetrics(painter.font()).lineSpacing();
    }

    painter.end();
}
