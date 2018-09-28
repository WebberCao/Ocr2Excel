#include "stdafx.h"


double TiltCorrection(int *image,int row,int column)
{
	/*//水平投影法获取 水平框线位置
	LengthRow *HorProject = new LengthRow[row];
	memset(HorProject, 0, sizeof(LengthRow) * row);
	for(int i = 0; i < row; i++)
	{
		HorProject[i].Row = i;
		for(int j = 0; j < column; j++)
		{
			if(image[i * column + j] == 0)
			{
				++HorProject[i].Length; 
			}
		}
	}
	//获取最大值，认为是框线的一部分
    LengthRow MaxLengthRow;
	MaxLengthRow.Length = 0;
	for(int i = 0; i < row; i++)
	{
		if(HorProject[i].Length > MaxLengthRow.Length)
		{
			MaxLengthRow = HorProject[i];
		}
	}

	//竖直投影法获取 竖直框线位置
	LengthColumn *VerProject = new LengthColumn[column];
	memset(VerProject, 0, sizeof(LengthColumn) * column);
		
	for(int j = 0; j < column; j++)
	{
		VerProject[j].Column = j;
		for(int i = 0; i < row; i++)
		{
			if(image[i * column + j] == 0)
			{
				++VerProject[j].Length; 
			}
		}
	}
	//获取最大值，认为是框线的一部分
    LengthColumn MaxLengthColumn;
	MaxLengthColumn.Length = 0;
	for(int i = 0; i < column; i++)
	{
		if(VerProject[i].Length > MaxLengthColumn.Length)
		{
			MaxLengthColumn = VerProject[i];
		}
	}

	double tiltAngle ;
	if(MaxLengthColumn.Length > MaxLengthRow.Length)
	{
		tiltAngle = FindTiltAngle_Ver(image,row,column);
	}
	else
	{
		tiltAngle =  - FindTiltAngle(image,row,column);
	}*/
	//tiltAngle = 180.0 
	//tiltAngle =  tiltAngle / 180 * PI;
	int maxlengthVer,maxlengthHor;
	double tiltAngleVer = - FindTiltAngle_Ver(image,row,column,maxlengthVer);
	double tiltAngleHor =  FindTiltAngle_Hor(image,row,column,maxlengthHor);

	double tiltAngle = maxlengthVer> maxlengthHor ? tiltAngleVer:tiltAngleHor;

	tiltAngle = (tiltAngleVer + tiltAngleHor) / 2.0;
	double cosAngle = cos(tiltAngle);
	double sinAngle = sin(tiltAngle);

	int center_i = row / 2;
	int center_j = column / 2;

	int *tempImage = new int[row * column];
	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < column; j++)
		{	
			tempImage[i * column + j] = 255;
		}
	}

	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < column; j++)
		{			
			double temp_i_f = (i - center_i) * cosAngle + (j - center_j) * sinAngle + center_i;
			double temp_j_f =  - (i - center_i) * sinAngle + (j - center_j) * cosAngle + center_j;

			int temp_i = int(temp_i_f);
			int temp_j = int(temp_j_f);

			if(temp_i < 0 || temp_j < 0 || temp_i >= row ||temp_j >= column)
				continue;

			if(image[i * column + j] == 0)
				tempImage[temp_i * column + temp_j] = 0;
		}
	}

	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < column; j++)
		{
			if(tempImage[i * column + j] > 255)
				image[i * column + j] = 255;
			else
				image[i * column + j] = tempImage[i * column + j];
		}
	}

	delete []tempImage;

	return tiltAngle;
}


double FindTiltAngle_Ver(int *image,int row,int column,int &maxlength)
{
	int *pArray;
 //
    int iRMax = (int)sqrt(double(row * row + column * column)) + 1;
 // this is diagonal line length
    int iThMax = 75 + 1;
 //  将直线的斜率分成360份，也即一周
    int iThStart = -75;
 //
    int iR;
    int iMax = -1;
    int iThMaxIndex = -1;
    int iRMaxIndex = -1;

    pArray = new int[iRMax * (iThMax - iThStart)];
    memset(pArray, 0, sizeof(int) * iRMax * (iThMax - iThStart));

    float fRate = (float)(0.1 * PI/180);

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            if(image[i * column + j] == 0)
		    //0 stands for foreground picture
            {
                for(int iTh = iThStart; iTh < iThMax; iTh += 1)
                {
                    iR = (int)(j * cos(iTh * fRate) + i * sin(iTh * fRate));

                    if(iR > 0)
                    {
                        pArray[iR * (iThMax - iThStart) + iTh - iThStart]++;
                    }
                }
            }
        } // x
    } // y

	//按长度排序 从大到小
	LengthAngle *_angleList = new LengthAngle[iRMax * (iThMax - iThStart)];
    memset(_angleList, 0, sizeof(LengthAngle) * iRMax * (iThMax - iThStart));
	// 这个是累积计算的过程，因为xy已知，当直线垂直于原点和xy点时候有最大的iR 
    for(iR = 0; iR < iRMax; iR++)
    {
        for(int iTh = iThStart; iTh < iThMax; iTh++)
        {
            int iCount = pArray[iR * (iThMax - iThStart) + iTh - iThStart];
			_angleList[iR * (iThMax - iThStart) + iTh - iThStart].Angle = iTh;
			_angleList[iR * (iThMax - iThStart) + iTh - iThStart].Length = iCount;			
        }
    }

	 //LengthquickSort(_angleList, 0, (iRMax * (iThMax - iThStart))); 

	/* LengthMaoPaoSort(_angleList,(iRMax * (iThMax - iThStart))); 	

	 double result ;
	 for(int k = iRMax * (iThMax - iThStart) - 1; k >=0;k--)
	 {
		 for(int kk = (k + 1);kk < iRMax * (iThMax - iThStart);kk++)
		 {
			 if(fabs(double(_angleList[kk].Angle - _angleList[k].Angle)) < 2)
			 {
				 result = double(_angleList[kk].Angle + _angleList[k].Angle) / 2.0 * fRate;
				 delete []pArray;
				 delete []_angleList;
				 return result;
			 }
		 }
	 }*/

	//寻找前MaxN大的Length
	int MaxN = 10;
	LengthAngle *MaxFive = new LengthAngle[MaxN];
	memset(MaxFive, 0, sizeof(LengthAngle) * MaxN);

	for(int k = 0;k < iRMax * (iThMax - iThStart);k++)
	{
		for(int kk = 0 ; kk < MaxN ; kk++)
		{
			if(_angleList[k].Length > MaxFive[kk].Length)
			{
				for(int kkk = MaxN - 1;kkk > kk;kkk--)
				{
					MaxFive[kkk] = MaxFive[kkk - 1];
				}
				 MaxFive[kk] = _angleList[k];
				 break;
			}
		}		
	}

	 for(int k = 0; k < MaxN; k++)
	 {
		 for(int kk = (k + 1);kk < MaxN;kk++)
		 {
			 if(fabs(double(MaxFive[kk].Angle - MaxFive[k].Angle)) < 2)
			 {
				 maxlength = MaxFive[k].Length;
				 double result = double(MaxFive[kk].Angle + MaxFive[k].Angle) / 2.0 * fRate;
				 delete []pArray;
				 delete []_angleList;
				 return result;
			 }
		 }
	 }

	delete []pArray;
	delete []_angleList;
	delete []MaxFive;
	return 0.0;
	 
}

double FindTiltAngle_Hor(int *image,int row,int column,int &maxlength)
{
	int *pArray;
 //
    int iRMax = (int)sqrt(double(row * row + column * column)) + 1;
 // this is diagonal line length
    int iThMax = 75 + 1;
 //  将直线的斜率分成360份，也即一周
    int iThStart = -75;
 //
    int iR;
    int iMax = -1;
    int iThMaxIndex = -1;
    int iRMaxIndex = -1;

    pArray = new int[iRMax * (iThMax - iThStart)];
    memset(pArray, 0, sizeof(int) * iRMax * (iThMax - iThStart));

    float fRate = (float)(0.1 * PI/180);

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            if(image[i * column + j] == 0)
		    //0 stands for foreground picture
            {
                for(int iTh = iThStart; iTh < iThMax; iTh += 1)
                {
                    iR = (int)(i * cos(iTh * fRate) + j * sin(iTh * fRate));

                    if(iR > 0)
                    {
                        pArray[iR * (iThMax - iThStart) + iTh - iThStart]++;
                    }
                }
            }
        } // x
    } // y

	//按长度排序 从大到小
	LengthAngle *_angleList = new LengthAngle[iRMax * (iThMax - iThStart)];
    memset(_angleList, 0, sizeof(LengthAngle) * iRMax * (iThMax - iThStart));
	// 这个是累积计算的过程，因为xy已知，当直线垂直于原点和xy点时候有最大的iR 
    for(iR = 0; iR < iRMax; iR++)
    {
        for(int iTh = iThStart; iTh < iThMax; iTh++)
        {
            int iCount = pArray[iR * (iThMax - iThStart) + iTh - iThStart];
			_angleList[iR * (iThMax - iThStart) + iTh - iThStart].Angle = iTh;
			_angleList[iR * (iThMax - iThStart) + iTh - iThStart].Length = iCount;			
        }
    }

	 //LengthquickSort(_angleList, 0, (iRMax * (iThMax - iThStart))); 

	/* LengthMaoPaoSort(_angleList,(iRMax * (iThMax - iThStart))); 	

	 double result ;
	 for(int k = iRMax * (iThMax - iThStart) - 1; k >=0;k--)
	 {
		 for(int kk = (k + 1);kk < iRMax * (iThMax - iThStart);kk++)
		 {
			 if(fabs(double(_angleList[kk].Angle - _angleList[k].Angle)) < 2)
			 {
				 result = double(_angleList[kk].Angle + _angleList[k].Angle) / 2.0 * fRate;
				 delete []pArray;
				 delete []_angleList;
				 return result;
			 }
		 }
	 }*/

	//寻找前MaxN大的Length
	int MaxN = 10;
	LengthAngle *MaxFive = new LengthAngle[MaxN];
	memset(MaxFive, 0, sizeof(LengthAngle) * MaxN);

	for(int k = 0;k < iRMax * (iThMax - iThStart);k++)
	{
		for(int kk = 0 ; kk < MaxN ; kk++)
		{
			if(_angleList[k].Length > MaxFive[kk].Length)
			{
				for(int kkk = MaxN - 1;kkk > kk;kkk--)
				{
					MaxFive[kkk] = MaxFive[kkk - 1];
				}
				 MaxFive[kk] = _angleList[k];
				 break;
			}
		}		
	}

	 for(int k = 0; k < MaxN; k++)
	 {
		 for(int kk = (k + 1);kk < MaxN;kk++)
		 {
			 if(fabs(double(MaxFive[kk].Angle - MaxFive[k].Angle)) < 2)
			 {
				 maxlength = MaxFive[k].Length;
				 double result = double(MaxFive[kk].Angle + MaxFive[k].Angle) / 2.0 * fRate;
				 delete []pArray;
				 delete []_angleList;
				 return result;
			 }
		 }
	 }

	delete []pArray;
	delete []_angleList;
	delete []MaxFive;
	return 0.0;
	 
}

void Hough(int *src,int width,int height, int *pR, int *pTh, int iThreshold)
// src is the src_img one_dimension array
// width is width of src_img, height  is height of src_img
// pR is distance from original_point to the line
// pTh is the angle of the line
// iThreshold value determines whether it is line or not
{
    int *pArray;
 //
    int iRMax = (int)sqrt(double(width * width + height * height)) + 1;
 // this is diagonal line length
    int iThMax = 361;
 //  将直线的斜率分成360份，也即一周
    int iTh = 0;
 //
    int iR;
    int iMax = -1;
    int iThMaxIndex = -1;
    int iRMaxIndex = -1;

    pArray = new int[iRMax * iThMax];
    memset(pArray, 0, sizeof(int) * iRMax * iThMax);

    float fRate = (float)(PI/180);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if(*src == 255)
   //255 stands for foreground picture
            {
                for(iTh = 0; iTh < iThMax; iTh += 1)
                {
                    iR = (int)(x * cos(iTh * fRate) + y * sin(iTh * fRate));

                    if(iR > 0)
                    {
                        pArray[iR/1 * iThMax + iTh]++;
                    }
                }
            }

            src++;
        } // x
    } // y
 // 这个是累积计算的过程，因为xy已知，当直线垂直于原点和xy点时候有最大的iR
 
    for(iR = 0; iR < iRMax; iR++)
    {
        for(iTh = 0; iTh < iThMax; iTh++)
        {
            int iCount = pArray[iR * iThMax + iTh];
            if(iCount > iMax)
            {
                iMax = iCount;
                iRMaxIndex = iR;
                iThMaxIndex = iTh;
            }
        }
    }
 // 找最大值的过程
    if(iMax >= iThreshold)
    {
        *pR = iRMaxIndex;
        *pTh = iThMaxIndex;
    }

    delete []pArray;

    return;
} // end of Hough


//按长度快速排序  从小到大升序排列
void LengthquickSort(LengthAngle s[], int l, int r)  
{  
    if (l< r)  
    {        
        int i = l, j = r;
		LengthAngle x = s[l];  
        while (i < j)  
        {  
            while(i < j && s[j].Length>= x.Length) // 从右向左找第一个小于x的数  
                j--;   
            if(i < j)  
                s[i++] = s[j];  
            while(i < j && s[i].Length< x.Length) // 从左向右找第一个大于等于x的数  
                i++;   
            if(i < j)  
                s[j--] = s[i];  
        }  
        s[i] = x;  
        LengthquickSort(s, l, i - 1); // 递归调用  
        LengthquickSort(s, i + 1, r);  
    }  
} 

//按长度快速排序  从小到大升序排列
void LengthMaoPaoSort(LengthAngle s[], int N)  
{ 
	for(int i=1;i<N-1;i++)
    {
        for(int j=0;j<N-i;j++)
        {
            if(s[j].Length>s[j+1].Length)
            {
				LengthAngle tmp=s[j];
                s[j]=s[j+1];
                s[j+1]=tmp;
            }
        }
    }
}

void _2DProjection(int *image,int row,int column,double tiltAngle)
{
	double cosAngle = cos(tiltAngle);
	double sinAngle = sin(tiltAngle);

	int *tempImage = new int[row * column];
	memset(tempImage, 0, sizeof(int) * row * column);

	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < column; j++)
		{
			if(image[i * column + j] == 0 )
				continue;
			double temp_i_f = i * cosAngle + j * sinAngle;
			double temp_j_f =  - i * sinAngle + j * cosAngle;

			int temp_i = int(temp_i_f);
			int temp_j = int(temp_j_f);

			if(temp_i < 0 || temp_j < 0 || temp_i >= row ||temp_j >= column)
				continue;

			tempImage[temp_i * column + temp_j] += (temp_i + 1 - temp_i_f)*(temp_j + 1-temp_j_f) * image[i * column + j];				
			
			if((temp_i + 1 < row))
			{
				tempImage[(temp_i + 1) * column + temp_j] += (temp_i_f-temp_i)*(temp_j+1-temp_j_f) * image[i * column + j];				
			}

			if(temp_j + 1 < column)
			{
				tempImage[temp_i * column + (temp_j + 1)] += (temp_i + 1 - temp_i_f)*(temp_j_f - temp_j) * image[i * column + j];				
			}

			if((temp_i + 1 < row) && (temp_j + 1 < column) )
			{
				tempImage[(temp_i + 1) * column + (temp_j + 1)] += (temp_i_f-temp_i)*(temp_j_f - temp_j) * image[i * column + j];				
			}
			//tempImage[temp_i * column + temp_j] += image[i * column + j];
		}
	}

	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < column; j++)
		{
			if(tempImage[i * column + j] > 255)
				image[i * column + j] = 255;
			else
				image[i * column + j] = tempImage[i * column + j];
		}
	}
	delete []tempImage;
}
