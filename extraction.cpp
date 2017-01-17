#include "extraction.h"
#include "ui_extraction.h"
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
int noofbits;
char sc[100];
long textsize,actualw,actualh,newh;

QString imagepath1;

extraction::extraction(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::extraction)
{
    ui->setupUi(this);
}

extraction::~extraction()
{
    delete ui;
}
int extraction::callcolor(gdImagePtr img,int color,int i)
{
    switch(i)
    {
    case 0:return gdImageRed (img, color);
    case 1:return gdImageGreen (img, color);
    case 2:return gdImageBlue (img, color);
    default:printf("exiting");exit(-1);
    }
}
void extraction::serial()
{
    freopen("/home/srinidhi/cimgextractionoutputserial.txt","w",stdout);

    omp_set_nested (1);

    FILE *fp = { 0 };
    gdImagePtr img;

    long  x, y, w, h;

    x = y = w = h = 0;


    //this works only for PNG

    if ((fp = fopen (imagepath1.toLatin1().constData(), "r")) == NULL)
        error (1, 0, "Error - fopen(): %s", "/home/srinidhi/picencrypted.png");
    else
        img = gdImageCreateFromPng (fp);
    w = gdImageSX (img);
    h = gdImageSY (img);



    boost::dynamic_bitset<> bitarray(actualw*h*3*noofbits);


    double t = omp_get_wtime ();
//#pragma omp parallel for
    for (x = 0; x < actualw-1; x++)
    {
        for (long y = 0; y < h; y++)
        {
            int colors[3] = { 0 };
            int color = gdImageGetPixel (img, x, y);
            for (int i = 0; i < 3; i++)
            {
                colors[i] = callcolor (img, color, i);
                for(int countbits=0;countbits < noofbits;countbits++)
                {
                    long accessindex=x*h*3*noofbits+y*3*noofbits+i*noofbits+countbits;
                    bitarray[accessindex]=(colors[i] & (1<<countbits))/(1<<countbits);
                }

            }
        }
    }
//#pragma omp barrier
    x=actualw-1;
//#pragma omp parallel for
    for (long y = 0; y < actualh; y++)
    {
        int colors[3] = { 0 };
        int color = gdImageGetPixel (img, x, y);
        for (int i = 0; i < 3; i++)
        {
            colors[i] = callcolor (img, color, i);
            for(int countbits=0;countbits < noofbits;countbits++)
            {
                long accessindex=x*h*3*noofbits+y*3*noofbits+i*noofbits+countbits;
                bitarray[accessindex]=(colors[i] & (1<<countbits))/(1<<countbits);
            }

        }
    }


    t = omp_get_wtime () - t;



    fclose (fp);


    gdImageDestroy (img);
    char result[100];
    sprintf (result,"\n\nSerial Time for extraction = %g\n", t);
    qDebug()<<result;


    long noofcharacters=textsize + 1;

    long count=0;
    for(long q=0;q<noofcharacters-1;q++)
    {
        char character=0;
        for(int m=7,n=0;m>=0;m--,n++)
        {
            character|= bitarray[q*8+m] << n;
        }

        cout<<character;
        count++;
    }

    cout<<endl<<count<<endl;
}

void extraction::parallel()
{
    freopen("/home/srinidhi/cimgextractionoutputparallel.txt","w",stdout);

    omp_set_nested (1);

    FILE *fp = { 0 };
    gdImagePtr img;

    long  x, y, w, h;

    x = y = w = h = 0;


    //this works only for PNG

    if ((fp = fopen (imagepath1.toLatin1().constData(), "r")) == NULL)
        error (1, 0, "Error - fopen(): %s", "/home/srinidhi/picencrypted.png");
    else
        img = gdImageCreateFromPng (fp);
    w = gdImageSX (img);
    h = gdImageSY (img);



    boost::dynamic_bitset<> bitarray(actualw*h*3*noofbits);


    double t = omp_get_wtime ();
#pragma omp parallel for
    for (x = 0; x < actualw-1; x++)
    {
        for (long y = 0; y < h; y++)
        {
            int colors[3] = { 0 };
            int color = gdImageGetPixel (img, x, y);
            for (int i = 0; i < 3; i++)
            {
                colors[i] = callcolor (img, color, i);
                for(int countbits=0;countbits < noofbits;countbits++)
                {
                    long accessindex=x*h*3*noofbits+y*3*noofbits+i*noofbits+countbits;
                    bitarray[accessindex]=(colors[i] & (1<<countbits))/(1<<countbits);
                }

            }
        }
    }
#pragma omp barrier
    x=actualw-1;
#pragma omp parallel for
    for (long y = 0; y < actualh; y++)
    {
        int colors[3] = { 0 };
        int color = gdImageGetPixel (img, x, y);
        for (int i = 0; i < 3; i++)
        {
            colors[i] = callcolor (img, color, i);
            for(int countbits=0;countbits < noofbits;countbits++)
            {
                long accessindex=x*h*3*noofbits+y*3*noofbits+i*noofbits+countbits;
                bitarray[accessindex]=(colors[i] & (1<<countbits))/(1<<countbits);
            }

        }
    }


    t = omp_get_wtime () - t;



    fclose (fp);


    gdImageDestroy (img);
    char result[100];
    sprintf (result,"\n\Parallel Time for extraction = %g\n", t);
    qDebug()<<result;



    long noofcharacters=textsize + 1;

    long count=0;
    for(long q=0;q<noofcharacters-1;q++)
    {
        char character=0;
        for(int m=7,n=0;m>=0;m--,n++)
        {
            character|= bitarray[q*8+m] << n;
        }

        cout<<character;
        count++;
    }

    cout<<endl<<count<<endl;
}

void extraction::on_pushButton_clicked()
{
    QString s = ui->password->text();
    string sr = s.toLatin1().constData();
    if(sr.size()!=21)
    {
        QMessageBox msgBox;
        msgBox.setText("Invalid Password! Looks like you are an intruder!");
        msgBox.exec();
    }
    else
    {
        int i=0,j=0;
        while(sc[i++]=sr[j++]);

        sscanf(sc,"%08ld%01d%04ld%04ld%04ld",&textsize,&noofbits,&actualw,&actualh,&newh);
        domyjob();
    }

}

void extraction::domyjob()
{
    if(imagepath1.isNull() == true)
    {
        QMessageBox msgBox;
        msgBox.setText("Invalid file. Please select a valid image file");
        msgBox.exec();
    }
    else
    {

        parallel();
        serial();
    }
}

void extraction::on_imagebrowse_clicked()
{
    //this->close();

    //use QDir::currentPath() for current path
    imagepath1 = QFileDialog::getOpenFileName(this, tr("Select an image file"), QDir::currentPath(),tr("Image Files (*.png)"));
    if ( imagepath1.isNull() == false )
    {
        //directory.setPath(path);
        qDebug()<<"path not null";
        qDebug()<<imagepath1;
        ui->imagepath1label->setText(imagepath1);
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
