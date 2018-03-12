#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

int negativo(int iMin, int iMax, int I)
{
	if(I<iMin)
	{
		return iMax;
	}
	else if(iMin <= I && I < iMax)
	{
		return ((iMin-iMax)/(iMax-iMin))*(I-iMin) + iMax;
	}
	else if(I >= iMax)
	{
		return iMin;
	}
	else
		printf("Não entrou em if\n");
	return 0;
}

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    Mat image;
    image = imread( argv[1], 1 );

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }

    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", image);

	double histImg1blue[256];
	double histImg1green[256];
	double histImg1red[256];
	int bMin = 300, bMax = -1, gMin = 300, gMax = -1, rMin = 300, rMax = -1; 

	for(int i=0; i<256; i++)
	{
		histImg1blue[i] = 0;
		histImg1green[i] = 0;
		histImg1red[i] = 0;
	}

    for (int y=0; y < image.rows; y++)
    	for (int x=0; x < image.cols; x++) 
	{
		Vec3b intensity = image.at<Vec3b>(y, x);
		histImg1blue[(int)(intensity.val[0])]++;
		histImg1green[(int)(intensity.val[1])]++;
		histImg1red[(int)(intensity.val[2])]++;

		if(bMin > (int)(intensity.val[0]))
			bMin = (int)(intensity.val[0]);
		else if(bMax < (int)(intensity.val[0]))
			bMax = (int)(intensity.val[0]);
		if(gMin > (int)(intensity.val[1]))
			gMin = (int)(intensity.val[1]);
		else if(gMax < (int)(intensity.val[1]))
			gMax = (int)(intensity.val[1]);
		if(rMin > (int)(intensity.val[2]))
			rMin = (int)(intensity.val[2]);
		else if(rMax < (int)(intensity.val[2]))
			rMax = (int)(intensity.val[2]);			
	}
	//Mat histPlot = cvCreateMat(256, 500, CV_8UC1);

	for(int i=0; i<256; i++)
	{	
		//int mag = histImg1blue[i];
		//line(histPlot,Point(i,0),Point(i,mag),Scalar(255,0,0));

		printf("%d: b: %lf, g: %lf, r: %lf\n", i, histImg1blue[i], histImg1green[i], histImg1red[i]);
	}

	for(int i=0; i<256; i++)
	{	
		histImg1blue[i] /= image.rows * image.cols;
		histImg1green[i] /= image.rows * image.cols;
		histImg1red[i] /= image.rows * image.cols;
	}

	double acumHistImg1blue[256] = {0}, acumHistImg1green[256] = {0}, acumHistImg1red[256] = {0};
	acumHistImg1blue[0] = histImg1blue[0];
	acumHistImg1green[0] = histImg1green[0];
	acumHistImg1red[0] = histImg1red[0];	
	for(int i=1; i<256; i++)
	{	
		acumHistImg1blue[i] = acumHistImg1blue[i-1] + histImg1blue[i];
		acumHistImg1green[i] = acumHistImg1green[i-1] + histImg1green[i];
		acumHistImg1red[i] = acumHistImg1red[i-1] + histImg1red[i];
				
		printf("%d: b: %lf, g: %lf, r: %lf\n", i, acumHistImg1blue[i], acumHistImg1green[i], acumHistImg1red[i]);
	}

	for (int y=0; y < image.rows; y++)
    	for (int x=0; x < image.cols; x++) 
	{
		Vec3b intensity = image.at<Vec3b>(y, x);
		

		///////////////////////////		IMAGEM EQUALIZADA		///////////////////////////
		uchar blue = acumHistImg1blue[intensity.val[0]]*255;
		uchar green = acumHistImg1green[intensity.val[1]]*255;
		uchar red = acumHistImg1red[intensity.val[2]]*255;
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		
		///////////////////////////		IMAGEM NEGATIVA			///////////////////////////
		/*
		//uchar blue = negativo(bMin, bMax, ((int)(intensity.val[0])));
		//uchar green = negativo(gMin, gMax, ((int)(intensity.val[1])));
		//uchar red = negativo(rMin, rMax, ((int)(intensity.val[2])));
		*/
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		image.at<Vec3b>(y,x) = Vec3b(blue,green,red);
	}

	namedWindow("Display Processed Image", WINDOW_AUTOSIZE );
    imshow("Display Processed Image", image);	

	//namedWindow("Hist",1);
	//imshow("Hist",histPlot);

    waitKey(0);

    return 0;
}

