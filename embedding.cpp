#include "embedding.h"
#include "ui_embedding.h"


#include<QFileDialog>
#include<qdebug.h>
#include<QMessageBox>
#include<QDir>

#include <iostream>       // std::cout
#include<stdio.h>
#include <bitset>         // std::bitset
#include <string>
#include<gd.h>
#include <boost/dynamic_bitset.hpp>
#include<error.h>
#include<omp.h>
#include<string.h>
//#include "/home/srinidhi/Downloads/CImg-1.6.6_pre093015/CImg.h"
//using namespace cimg_library;
using namespace std;

QString imagepath="Please select an image file";
QString textpath="Please select a text file";
char *mainstr;
char *str;
string text;

embedding::embedding(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::embedding)
{
    ui->setupUi(this);

}

embedding::~embedding()
{
    delete ui;

}

int embedding::callcolor(gdImagePtr img,int color,int i)
{
    switch(i)
    {
    case 0:return gdImageRed (img, color);
    case 1:return gdImageGreen (img, color);
    case 2:return gdImageBlue (img, color);
    default:printf("exiting");exit(-1);
    }
}
void embedding::on_imagebrowse_clicked()
{
    //this->close();

    //use QDir::currentPath() for current path
    imagepath = QFileDialog::getOpenFileName(this, tr("Select an image file"), QDir::currentPath(),tr("Image Files (*.png)"));
    if ( imagepath.isNull() == false )
    {
        //directory.setPath(path);
        qDebug()<<"path not null";
        qDebug()<<imagepath;
        ui->label->setText(imagepath);
    }
    else
    {
        qDebug()<<"path null\n";
        QMessageBox msgBox;
        msgBox.setText("Invalid file selected. Please select a valid image file");
        msgBox.exec();
        //can open the browse again here

    }
}

void embedding::on_textbrowse_clicked()
{
    //use QDir::currentPath() for current path
    textpath = QFileDialog::getOpenFileName(this, tr("Select a text file"), QDir::currentPath(),tr("Text files (*.txt)"));
    if ( textpath.isNull() == false )
    {
        //directory.setPath(path);
        qDebug()<<"path not null";
        qDebug()<<textpath;
        ui->label_2->setText(textpath);

    }
    else
    {
        qDebug()<<"path null\n";
        QMessageBox msgBox;
        msgBox.setText("Invalid file selected. Please select a valid text file");
        msgBox.exec();
        //can open the browse again here

    }
}

void embedding::readmyfile()
{
    FILE *fp=fopen(textpath.toLatin1().constData(),"r");
    if(fp==NULL) exit(printf("something's wrong! "));
    mainstr=new char[1000000000];
    str=new char[1000];
    while(fgets(str,1000,fp)!=NULL)
        strcat(mainstr,str);
    fclose(fp);
    //puts(mainstr);
    text=mainstr;
}

void embedding::serial()
{

    omp_set_nested (1);
    int noofbits=1;

    //long textbitsize=((text.size()*8 +7)/8)*8;
    long textbitsize=((text.size()*8 + (3*noofbits)-1)/(3*noofbits))*(3*noofbits);
    boost::dynamic_bitset<> bitarray(textbitsize); // all 0's by default

    //for (boost::dynamic_bitset<>::size_type i = 0; i < x.size(); ++i)

    for(unsigned long j=0;j<text.size();j++)
        for(unsigned long k=0,q=7;k<8;k++,q--)
        {
            bitarray[j*8+k]=(text[j] & (1<<q))/(1<<q);
            //cout<<text[j]<<" "<<bitarray[j*8+k];
            //cout<<endl;
        }
    //cout<<x<<endl;


    FILE *fp = { 0 };
    gdImagePtr img;
    char *iname = NULL;
    char *oname = NULL;
    long x, y, w, h;
    //int red, green, blue;
    //int colors[3]={0};
    x = y = w = h = 0;
    //red = green = blue = 0;



    if ((fp = fopen (imagepath.toLatin1().constData(), "r")) == NULL)
        error (1, 0, "Error - fopen(): %s", iname);
    else
        img = gdImageCreateFromPng (fp);
    w = gdImageSX (img);
    h = gdImageSY (img);

    while(unsigned(w*h*3*noofbits)<text.size()*8)
    {
        noofbits++;
    }
    if(noofbits>8)
    {
        gdImageDestroy (img);
        fclose (fp);
        QMessageBox msgBox;
        msgBox.setText("Data too large to embed. Try with a smaller file");
        msgBox.exec();
        return;
    }




    long actualw=(text.size()*8)/(h*3*noofbits) +1;
    long actualh=(((text.size()*8)/(3*noofbits)) +1)%h;
    long newh=h;
    char pass[100];
    //printf("%08ld%01d",text.size(),noofbits);
    //printf("%04ld%04ld%04ld",actualw,actualh,newh);

    sprintf(pass,"%08ld%01d%04ld%04ld%04ld\0",text.size(),noofbits,actualw,actualh,newh);
    qDebug()<<pass;
    ui->password->setText(pass);

    double t = omp_get_wtime ();
//#pragma omp parallel for
    for (x = 0; x < actualw-1; x++)
    {
        for (long y = 0; y < newh; y++)
        {
            int colors[3] = { 0 };
            int color = gdImageGetPixel (img, x, y);
            for (int i = 0; i < 3; i++)
            {
                colors[i] = callcolor (img, color, i);
                for(int countbits=0;countbits < noofbits;countbits++)
                {
                    long accessindex=x*h*3*noofbits+y*3*noofbits+i*noofbits+countbits;
                    colors[i] &= ~(1<<countbits);
                    if(bitarray[accessindex]==1)
                        colors[i] |= (1<<countbits);
                }
            }

            color = gdImageColorAllocate (img, colors[0], colors[1], colors[2]);
            gdImageSetPixel (img, x, y, color);
        }
    }
    newh=actualh;x=actualw-1;
//#pragma omp parallel for
    for ( y = 0; y < newh; y++)
    {
        int colors[3] = { 0 };
        int color = gdImageGetPixel (img, x, y);
        for (int i = 0; i < 3; i++)
        {
            colors[i] = callcolor (img, color, i);
            for(int countbits=0;countbits < noofbits;countbits++)
            {
                long accessindex=x*h*3*noofbits+y*3*noofbits+i*noofbits+countbits;
                colors[i] &= ~(1<<countbits);
                if(bitarray[accessindex]==1)
                    colors[i] |= (1<<countbits);
            }
        }

        color = gdImageColorAllocate (img, colors[0], colors[1], colors[2]);
        gdImageSetPixel (img, x, y, color);
    }
    t = omp_get_wtime () - t;

    if ((fp = fopen ("/home/srinidhi/picinvertedserial.png", "wb")) == NULL)
        error (1, 0, "Error - fopen(): %s", oname);
    else
    {
        gdImagePng (img, fp);
        fclose (fp);
    }

    gdImageDestroy (img);
    char result[100];
    sprintf (result,"\n\n Serial Time for insertion = %g\n\0", t);
    ui->serial->setText(result);
    qDebug()<<result;
    //QString s=ui->serial->text();
    //s.append(QByteArray::fromRawData(reinterpret_cast<const char*>(&t),sizeof(double)));
    //ui->serial->setText(s);

}

void embedding::parallel()
{

    omp_set_nested (1);
    int noofbits=1;

    //long textbitsize=((text.size()*8 +7)/8)*8;
    long textbitsize=((text.size()*8 + (3*noofbits)-1)/(3*noofbits))*(3*noofbits);
    boost::dynamic_bitset<> bitarray(textbitsize); // all 0's by default

    //for (boost::dynamic_bitset<>::size_type i = 0; i < x.size(); ++i)

    for(unsigned long j=0;j<text.size();j++)
        for(unsigned long k=0,q=7;k<8;k++,q--)
        {
            bitarray[j*8+k]=(text[j] & (1<<q))/(1<<q);
            //cout<<text[j]<<" "<<bitarray[j*8+k];
            //cout<<endl;
        }
    //cout<<x<<endl;


    FILE *fp = { 0 };
    gdImagePtr img;
    char *iname = NULL;
    char *oname = NULL;
    long x, y, w, h;
    //int red, green, blue;
    //int colors[3]={0};
    x = y = w = h = 0;
    //red = green = blue = 0;



    if ((fp = fopen (imagepath.toLatin1().constData(), "r")) == NULL)
        error (1, 0, "Error - fopen(): %s", iname);
    else
        img = gdImageCreateFromPng (fp);
    w = gdImageSX (img);
    h = gdImageSY (img);

    while(unsigned(w*h*3*noofbits)<text.size()*8)
    {
        noofbits++;
    }
    if(noofbits>8)
    {
        gdImageDestroy (img);
        fclose (fp);
        QMessageBox msgBox;
        msgBox.setText("Data too large to embed. Try with a smaller file");
        msgBox.exec();
        return;
    }

    //printf("%08ld%01d",text.size(),noofbits);

    long actualw=(text.size()*8)/(h*3*noofbits) +1;
    long actualh=(((text.size()*8)/(3*noofbits)) +1)%h;
    long newh=h;
    //printf("%04ld%04ld%04ld",actualw,actualh,newh);
    //printf("%08ld%01d",text.size(),noofbits);
    //printf("%04ld%04ld%04ld",actualw,actualh,newh);
    char pass[100];
    sprintf(pass,"%08ld%01d%04ld%04ld%04ld\0",text.size(),noofbits,actualw,actualh,newh);
    qDebug()<<pass;
    ui->password->setText(pass);
    double t = omp_get_wtime ();
#pragma omp parallel for
    for (x = 0; x < actualw-1; x++)
    {
        for (long y = 0; y < newh; y++)
        {
            int colors[3] = { 0 };
            int color = gdImageGetPixel (img, x, y);
            for (int i = 0; i < 3; i++)
            {
                colors[i] = callcolor (img, color, i);
                for(int countbits=0;countbits < noofbits;countbits++)
                {
                    long accessindex=x*h*3*noofbits+y*3*noofbits+i*noofbits+countbits;
                    colors[i] &= ~(1<<countbits);
                    if(bitarray[accessindex]==1)
                        colors[i] |= (1<<countbits);
                }
            }

            color = gdImageColorAllocate (img, colors[0], colors[1], colors[2]);
            gdImageSetPixel (img, x, y, color);
        }
    }
    newh=actualh;x=actualw-1;
#pragma omp parallel for
    for ( y = 0; y < newh; y++)
    {
        int colors[3] = { 0 };
        int color = gdImageGetPixel (img, x, y);
        for (int i = 0; i < 3; i++)
        {
            colors[i] = callcolor (img, color, i);
            for(int countbits=0;countbits < noofbits;countbits++)
            {
                long accessindex=x*h*3*noofbits+y*3*noofbits+i*noofbits+countbits;
                colors[i] &= ~(1<<countbits);
                if(bitarray[accessindex]==1)
                    colors[i] |= (1<<countbits);
            }
        }

        color = gdImageColorAllocate (img, colors[0], colors[1], colors[2]);
        gdImageSetPixel (img, x, y, color);
    }
    t = omp_get_wtime () - t;

    if ((fp = fopen ("/home/srinidhi/picinvertedparallel.png", "wb")) == NULL)
        error (1, 0, "Error - fopen(): %s", oname);
    else
    {
        gdImagePng (img, fp);
        fclose (fp);
    }

    gdImageDestroy (img);
    char result[100];
    sprintf (result,"\n\n Parallel Time for insertion = %g\n\0", t);
    ui->parallel->setText(result);
    qDebug()<<result;
    //QString s=ui->parallel->text();
    //s.append(QByteArray::fromRawData(reinterpret_cast<const char*>(&t),sizeof(double)));
    //ui->parallel->setText(s);

}

void embedding::callmyfunctions()
{
    readmyfile();
    parallel();
    serial();


}

void embedding::on_embed_clicked()
{
    if(imagepath.isNull()==true || textpath.isNull()==true)
    {
        qDebug()<<"path null\n";
        QMessageBox msgBox;
        if(imagepath.isNull()==true)
        {
            msgBox.setText("Invalid file selected. Please select a valid image file");
            msgBox.exec();
        }
        if(textpath.isNull()==true)
        {
            msgBox.setText("Invalid file selected. Please select a valid text file");
            msgBox.exec();
        }

    }
    else
        callmyfunctions();
}
