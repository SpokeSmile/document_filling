#include "pdfgenerator.h"
#include <QPdfWriter>
#include <QPainter>
#include <QFontMetrics>
#include <QRectF>
#include <QFontDatabase>
#include <iostream>

// Инициализация объекта генератора PDF
PdfGenerator::PdfGenerator(QObject *parent) :
    QObject(parent)
{}

void PdfGenerator::generatePdf(const QString &filename, const std::vector<Student> &students)
{
    QPdfWriter writer(filename);
    writer.setPageSize(QPageSize::A4);                          // Формат А4
    writer.setResolution(72);                                   // Разрешение 72 dpi

    QPainter painter(&writer);
    painter.setPen(Qt::black);

    // Шрифты для шапки и тела документа
    QFont smallRegularFont("Arial", 10);                        // Мелкий шрифт для шапки
    QFont boldSmallFont("Arial", 12, QFont::Bold);              // Жирный мелкий шрифт для названия колледжа

    // Размер страницы
    double pageWidth = writer.width();                           // Ширина страницы в пикселях
    double pageHeight = writer.height();                         // Высота страницы в пикселях

    // Преобразуем мм в пиксели
    constexpr double mmToPxFactor = 72.0 / 25.4;                // коэффициент конвертации мм в пиксели
    double maxContentWidth = 95 * mmToPxFactor;                 // Ограничиваем информацию по ширине (≈ 270 px)

    // Ограничительные прямоугольники для шапки (сдвинутые по центру)
    QRectF topTitleRect((pageWidth - maxContentWidth)/2, 50, maxContentWidth, 20);
    QRectF middleTitleRect((pageWidth - maxContentWidth)/2, 70, maxContentWidth, 20);
    QRectF bottomTitleRect((pageWidth - maxContentWidth)/2, 90, maxContentWidth, 20);

   
    // Основной блок документа (начиная с отметки 150 пикселей по оси Y)
    int yPosition = 150;

    for (const Student &student : students) {
         // Шапка документа
        QString topLine = "АВТОНОМНАЯ НЕКОММЕРЧЕСКАЯ ОРГАНИЗАЦИЯ";
        QString middleLine = "ПРОФЕССИОНАЛЬНАЯ ОБРАЗОВАТЕЛЬНАЯ ОРГАНИЗАЦИЯ";
        QString bottomLine = "МОСКОВСКИЙ МЕЖДУНАРОДНЫЙ КОЛЛЕДЖ ЦИФРОВЫХ ТЕХНОЛОГИЙ \"АКАДЕМИЯ ТОП\"";

        // Ставим мелкие символы для шапки
        painter.setFont(smallRegularFont);
        painter.drawText(topTitleRect, Qt::AlignHCenter, topLine);
        painter.drawText(middleTitleRect, Qt::AlignHCenter, middleLine);

        painter.setFont(boldSmallFont);
        painter.drawText(bottomTitleRect, Qt::AlignHCenter, bottomLine);

        // Данные студента
        QString ticketNum = QString("%1").arg(student.getTicketNumber());
        QString surname = student.getLastName();
        QString firstName = student.getFirstName();
        QString patronymic = student.getMiddleName();

        // Используем два разных шрифта
        QFont boldBodyFont("Arial", 12, QFont::Bold);                       // Жирный шрифт
        QFont bodyFont("Arial", 12);                                        // Обычный шрифт

        // Зона для вывода информации справа (оставляя слева место для фотографии)
        qreal textStartX = pageWidth - maxContentWidth - 50;                // Начало координат справа
        QRectF contentRect(textStartX, yPosition, maxContentWidth, 200);    // Зона вывода (строго 95 мм ≈ 270px)

        // Выводим данные студента
        painter.setFont(boldBodyFont);
        painter.drawText(contentRect.topLeft(), "Студентский билет №");
        painter.setFont(bodyFont);
        painter.drawText(contentRect.topLeft() + QPointF(140, 0), ticketNum); // выводим номер билета

        // Вывод остальных данных
        painter.setFont(bodyFont);
        painter.drawText(contentRect.topLeft() + QPointF(0, 20), "Фамилия: " + surname);       // Фамилия
        painter.drawText(contentRect.topLeft() + QPointF(0, 40), "Имя: " + firstName);         // Имя
        painter.drawText(contentRect.topLeft() + QPointF(0, 60), "Отчество: " + patronymic);   // Отчество
        painter.drawText(contentRect.topLeft() + QPointF(0, 80), QString("Форма обучения: %1").arg(student.getFormOfStudy()));
        painter.drawText(contentRect.topLeft() + QPointF(0, 100), "Зачислен приказом: 30.08.2024");
        painter.drawText(contentRect.topLeft() + QPointF(0, 120), "Дата выдачи: 30 сентября 2024");

        // Подпись студента под остальными строками
        QRectF signatureRect(textStartX -280, yPosition + 140, maxContentWidth, 50);  // Сдвигаем под последнюю строку
        painter.drawText(signatureRect.bottomRight() - QPointF(-20,-10), "Подпись студента:");

        // Подпись руководителя слева
        QRectF directorSignatureRect(50, yPosition + 200, maxContentWidth, 50);  // Сдвигаем сильно влево
        painter.drawText(directorSignatureRect.bottomLeft() + QPointF(0, -10),
                         "Руководитель образовательной организации или уполномоченное им должностное лицо");

        // Далее перемещаемся ниже
        yPosition += 250;
    }

    painter.end();
}
