

#include <math.h>
#define PI 3.141592654


struct LengthAngle
{
	int Length;
	int Angle;
};

double TiltCorrection(int *image,int row,int column);
double FindTiltAngle_Ver(int *image,int row,int column,int &maxlength);
void Hough(BYTE *src,int width,int height, int *pR, int *pTh, int iThreshold);
void LengthquickSort(LengthAngle s[], int l, int r);
void LengthMaoPaoSort(LengthAngle s[], int N);
double FindTiltAngle_Hor(int *image,int row,int column,int &maxlength);