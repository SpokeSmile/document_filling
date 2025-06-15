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
    QFont smallRegularFont("Times New Roman", 9);                        // Мелкий шрифт для шапки
    QFont boldSmallFont("Times New Roman", 9, QFont::Bold);              // Жирный мелкий шрифт для названия колледжа

    // Размер страницы
    double pageWidth = writer.width();                           // Ширина страницы в пикселях
    double pageHeight = writer.height();                         // Высота страницы в пикселях

    // Преобразуем мм в пиксели
    constexpr double mmToPxFactor = 72.0 / 25.4;                // коэффициент конвертации мм в пиксели
    double maxContentWidth = 95 * mmToPxFactor;                 // Ограничиваем информацию по ширине (≈ 270 px)

    
    
    int yPosition = 0;
    int xPosition = 0;
    constexpr int spaceNeeded = 250;
    constexpr int bottomMargin = 50;
    for (const Student &student : students) {
        // Проверка перехода на новую страницу перед отрисовкой студента
        if (yPosition + spaceNeeded > pageHeight - bottomMargin) {
            writer.newPage();
            yPosition = 150;
        }
        // Переносим шапку сюда, чтобы рисовать её для каждого студента отдельно
        // Ограничительные прямоугольники для шапки (сдвинутые по центру)
        QRectF topTitleRect(xPosition, yPosition, maxContentWidth, 30);
        QRectF middleTitleRect(xPosition, yPosition + 20, maxContentWidth, 30);
        QRectF bottomTitleRect(xPosition, yPosition + 40, maxContentWidth, 30);
        // Шапка документа
        QString topLine = "АВТОНОМНАЯ НЕКОММЕРЧЕСКАЯ ОРГАНИЗАЦИЯ";
        QString middleLine = "ПРОФЕССИОНАЛЬНАЯ ОБРАЗОВАТЕЛЬНАЯ ОРГАНИЗАЦИЯ";
        QString bottomLine = "МОСКОВСКИЙ МЕЖДУНАРОДНЫЙ КОЛЛЕДЖ ЦИФРОВЫХ ТЕХНОЛОГИЙ \"АКАДЕМИЯ ТОП\"";

         // Рисуем шапку
        painter.setFont(smallRegularFont);
        painter.drawText(topTitleRect, Qt::AlignHCenter | Qt::AlignTop, topLine);
        painter.drawText(middleTitleRect, Qt::AlignHCenter | Qt::AlignTop, middleLine);

        painter.setFont(boldSmallFont);
        painter.drawText(bottomTitleRect, Qt::AlignHCenter | Qt::AlignTop, bottomLine);

        // Основная информация о студенте начинается немного ниже шапки
        yPosition += 60; // Смещаем позицию вниз, чтобы освободить место для шапки

        QString ticketNum = QString("%1").arg(student.getTicketNumber());
        QString surname = student.getLastName();
        QString firstName = student.getFirstName();
        QString patronymic = student.getMiddleName();
        // Используем два разных шрифта
        QFont boldBodyFont("Times New Roman", 10, QFont::Bold);                       // Жирный шрифт
        QFont bodyFont("Times New Roman", 10);                                        // Обычный шрифт

        // Зона для вывода информации справа (оставляя слева место для фотографии)
        qreal textStartX = pageWidth - maxContentWidth - 50;                // Начало координат справа
        QRectF contentRect(textStartX, yPosition+10, maxContentWidth, 200);    // Зона вывода (строго 95 мм ≈ 270px)

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
