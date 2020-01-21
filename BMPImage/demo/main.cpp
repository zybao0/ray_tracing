#include<iostream>
#include<BMPImage.h>
using namespace std;
int main()
{
	BMPImage bmpImage(200,100,32);
	for(int i=0;i<100;i++)
	for(int j=0;j<200;j++)
	{
		bmpImage.at<BGRA>(i, j) = BGRA{(int)(255.99*0.2),(int)(255.99*float(j)/float(200)),(int)(255.99*float(i)/float(100)),0};
	}
	bmpImage.save("hello.bmp");
}