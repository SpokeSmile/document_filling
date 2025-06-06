#ifndef PDFGENERATOR_H
#define PDFGENERATOR_H

#include <QString>
#include <QObject>
#include <vector>
#include "structs.h"

class PdfGenerator : public QObject
{
    Q_OBJECT
public:
    explicit PdfGenerator(QObject *parent = nullptr);

    /**
     * @brief Метод генерирует PDF-файл с информацией о студенте
     *
     * @param filename Имя файла для сохранения
     * @param students Список отобранных студентов
     */
    void generatePdf(const QString& filename, const std::vector<Student>& students);
};

#endif // PDFGENERATOR_H
