#include "mainwindow.h"
#include <QApplication>
// bitset::set
#include <iostream>       // std::cout
#include<stdio.h>
#include <bitset>         // std::bitset
#include <string>
#include<gd.h>
#include <boost/dynamic_bitset.hpp>
#include<error.h>
#include<omp.h>
#include<string.h>
#include<QMessageBox>
//#include "/home/srinidhi/Downloads/CImg-1.6.6_pre093015/CImg.h"
//using namespace cimg_library;
using namespace std;
int callcolor(gdImagePtr img,int color,int i)
{
    switch(i)
    {
    case 0:return gdImageRed (img, color);
    case 1:return gdImageGreen (img, color);
    case 2:return gdImageBlue (img, color);
    default:printf("exiting");exit(-1);
    }
}
int getbit(char ch,int i)
{
    return ch&&1<<i;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w1;
    w1.show();
/*
    FILE *fp1=fopen("/home/srinidhi/HPC/history3.txt","r");
    if(fp1==NULL) exit(printf("something's wrong! "));
    char *mainstr=new char[1000000000];
    char *str=new char[1000];
    while(fgets(str,1000,fp1)!=NULL)
        strcat(mainstr,str);
    fclose(fp1);
    //puts(mainstr);

    omp_set_nested (1);
    int noofbits=1;

    string text;
    //cin>>text;
    text="Srinidhi Karthik BS password generation";
    text=mainstr;

    //text="#runfile('/home/srinidhi/mini project/testingandexperimenting.py', wdir=r'/home/srinidhi/mini project')runfile('/home/srinidhi/mini project/testingandexperimenting.py', wdir=r'/home/srinidhi/mini project')runfile('/home/srinidhi/mini project/testingandexperimenting.py', wdir=r'/home/srinidhi/mini project')runfile('/home/srinidhi/mini project/testingandexperimenting.py', wdir=r'/home/srinidhi/mini project')";
    cout<<text.size();

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
    //cout<<endl<<endl<<bitarray.size();

    cout<<endl<<endl<<endl;







    FILE *fp = { 0 };
    gdImagePtr img;
    char *iname = NULL;
    char *oname = NULL;
    long x, y, w, h;
    //int red, green, blue;
    //int colors[3]={0};
    x = y = w = h = 0;
    //red = green = blue = 0;



    if ((fp = fopen ("/home/srinidhi/pic.png", "r")) == NULL)
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
        return 0;
    }

    printf("%08ld%01d",text.size(),noofbits);

    long actualw=(text.size()*8)/(h*3*noofbits) +1;
    long actualh=(((text.size()*8)/(3*noofbits)) +1)%h;
    long newh=h;
    printf("%04ld%04ld%04ld",actualw,actualh,newh);
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

    if ((fp = fopen ("/home/srinidhi/picinverted.png", "wb")) == NULL)
        error (1, 0, "Error - fopen(): %s", oname);
    else
    {
        gdImagePng (img, fp);
        fclose (fp);
    }

    gdImageDestroy (img);
    printf ("\n\nTime taken = %g\n", t);

*/

    return a.exec();
  //  return 0;
}
