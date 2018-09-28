#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void ImageBinaryzation(int *image,int row,int column);
int OtsuMethod(int *image,int row,int column);
int PartThMethod(int *Image, int row, int column, int i0, int j0,int Thg);
int QuarterThMethod(int *Image, int row, int column, int i0, int j0,int Thg);
void quickSort(int s[], int l, int r);