#ifndef EXTRACTION_H
#define EXTRACTION_H

#include <QMainWindow>
#include<gd.h>
namespace Ui {
class extraction;
}

class extraction : public QMainWindow
{
    Q_OBJECT

public:
    explicit extraction(QWidget *parent = 0);
    ~extraction();

private slots:
    void on_pushButton_clicked();

    void on_imagebrowse_clicked();

public slots:
    int callcolor(gdImagePtr,int,int);
    void domyjob();
    void serial();
    void parallel();



private:
    Ui::extraction *ui;
};

#endif // EXTRACTION_H
