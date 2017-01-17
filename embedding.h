#ifndef EMBEDDING_H
#define EMBEDDING_H

#include <QMainWindow>
#include<gd.h>
namespace Ui {
class embedding;
}

class embedding : public QMainWindow
{
    Q_OBJECT

public:
    explicit embedding(QWidget *parent = 0);
    ~embedding();

private slots:


    void on_imagebrowse_clicked();

    void on_textbrowse_clicked();

    void on_embed_clicked();

public slots:
    void callmyfunctions();
    void serial();
    void parallel();
    void readmyfile();
    int callcolor(gdImagePtr,int,int);


private:
    Ui::embedding *ui;
};

#endif // EMBEDDING_H
