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

    double pageWidth = writer.width();                           // Ширина страницы в пикселях
    double pageHeight = writer.height();                         // Высота страницы в пикселях

    // Нарисовать вертикальную линию по центру страницы
    QPen centerLinePen(Qt::black);
    centerLinePen.setWidth(1);
    painter.setPen(centerLinePen);
    painter.drawLine(QPointF(pageWidth / 2, 0), QPointF(pageWidth / 2, pageHeight));

    // !Инициализация шрифтов!
    // Создаем метрики шрифтов для улучшенного отображения
    QFont smallHeadings("Times New Roman", 6);
    QFont boldHeadings("Times New Roman", 6, QFont::Bold);
    QFontMetrics smallHeadingsMetrics(smallHeadings);
    QFontMetrics boldHeadingsMetrics(boldHeadings);

    QFont smallBodyFont("Times New Roman", 6);
    QFont boldBodyFont("Times New Roman", 6, QFont::Bold);
    QFontMetrics smallBodyFontMetrics(smallBodyFont);
    QFontMetrics boldBodyFontMetrics(boldBodyFont);
    
    QFont smallInfoFont("Times New Roman", 4);
    QFontMetrics smallInfoFontMetrics(smallInfoFont);

    int yPosition = -5;
    std::cout << pageHeight << " " <<pageWidth;

    constexpr int spaceNeeded = 250;
    constexpr int bottomMargin = 50;
    double halfPageWidth = pageWidth / 2;

    // Нарисовать горизонтальные линии через каждые 184 пикселя
    // ПЕРЕДЕЛАТЬ РИСУЕТ НЕ НА ВСЕХ СТРАНИЦАХ
    painter.setPen(QPen(Qt::black, 0.2));
    for (int y = 184; y < pageHeight; y += 184) {
        painter.drawLine(QPointF(0, y), QPointF(pageWidth, y));
    }

    for (const Student &student : students) {
        // Проверка перехода на новую страницу перед отрисовкой студента
        if (yPosition + spaceNeeded > pageHeight - bottomMargin) {
            writer.newPage();
            yPosition = 150;
        }
        
        // Шапка документа
        QString topLine = "АВТОНОМНАЯ НЕКОММЕРЧЕСКАЯ ОРГАНИЗАЦИЯ";
        QString middleLine = "ПРОФЕССИОНАЛЬНАЯ ОБРАЗОВАТЕЛЬНАЯ ОРГАНИЗАЦИЯ";
        QString bottomLine = "МОСКОВСКИЙ МЕЖДУНАРОДНЫЙ КОЛЛЕДЖ ЦИФРОВЫХ ТЕХНОЛОГИЙ \"АКАДЕМИЯ ТОП\"";
        // 
        QRectF topTitleRect(-5, yPosition, halfPageWidth, smallHeadingsMetrics.height());
        QRectF middleTitleRect(-5, yPosition += 7, halfPageWidth, smallHeadingsMetrics.height());
        QRectF bottomTitleRect(-5, yPosition += 7, halfPageWidth, boldHeadingsMetrics.height());

        painter.setFont(smallHeadings);
        painter.drawText(topTitleRect, Qt::AlignHCenter | Qt::AlignTop, topLine);
        painter.drawText(middleTitleRect, Qt::AlignHCenter | Qt::AlignTop, middleLine);

        painter.setFont(boldHeadings);
        painter.drawText(bottomTitleRect, Qt::AlignHCenter | Qt::AlignTop, bottomLine);

        // Основная информация о студенте начинается немного ниже шапки
        
        
        // Зона для вывода информации справа (оставляя слева место для фотографии)
        qreal textStartX = 100;                // Начало координат справа
        QRectF contentRect(textStartX, yPosition += 20, halfPageWidth, boldHeadingsMetrics.height());    // Зона вывода (строго 95 мм ≈ 270px)

        // Выводим данные студента
        painter.setFont(boldBodyFont);
        painter.drawText(contentRect.topLeft(), "Студентский билет №");
        contentRect.setHeight(smallBodyFontMetrics.height());
        painter.setFont(smallBodyFont);
        painter.drawText(contentRect.topLeft() + QPointF(60, 0), student.getTicketNumber()); // выводим номер билета

        // Вывод остальных данных
        painter.setFont(smallBodyFont);
        painter.drawText(contentRect.topLeft() + QPointF(0, 15), "Фамилия " + student.getLastName());       // Фамилия
        painter.drawText(contentRect.topLeft() + QPointF(0, 30), "Имя " + student.getFirstName());         // Имя
        painter.drawText(contentRect.topLeft() + QPointF(0, 45), "Отчество " + student.getMiddleName());   // Отчество
        painter.drawText(contentRect.topLeft() + QPointF(0, 60), QString("Форма обучения" + student.getFormOfStudy()));
        painter.drawText(contentRect.topLeft() + QPointF(0, 75), "Зачислен приказом" + student.getOrderDate() + " №" + student.getOrderNumber());
        painter.drawText(contentRect.topLeft() + QPointF(0, 90), "Дата выдачи" + student.getIssueDate());

        painter.setFont(smallInfoFont);
        // Подпись студента под остальными строками
        QRectF signatureRect(textStartX, yPosition += 120, halfPageWidth, smallInfoFontMetrics.height());  // Сдвигаем под последнюю строку
        painter.drawText(signatureRect.bottomRight() - QPointF(-20,-10), "Подпись студента:");

        // Подпись руководителя слева (увеличенная высота для 6 строк текста с переносами)
        QRectF directorSignatureRect(-5, yPosition += 10, halfPageWidth, smallInfoFontMetrics.lineSpacing() * 6);  // Сдвигаем сильно влево и увеличиваем высоту
        QString directorText = "Руководитель организации,\nосуществляющей\nобразовательную\nдеятельность, или иное\nуполномоченное имя,\nдолжностное лицо";
        painter.drawText(directorSignatureRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, directorText);

        // Далее перемещаемся ниже
        yPosition += 250;
    }

    painter.end();
}
