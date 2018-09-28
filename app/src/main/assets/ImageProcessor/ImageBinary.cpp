#include "stdafx.h"


void ImageBinaryzation(int *image,int row,int column)
{
	//计算全局阈值Thg
	int Thg = OtsuMethod(image,row,column); 

	//二值化处理
	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < column; j++)
		{
			if(image[i * column + j] > Thg) //背景
			{
				image[i * column + j] = 255;
			}
			else
			{
				if(image[i * column + j] >= QuarterThMethod(image, row, column, i, j, Thg))
				{
					if(image[i * column + j] > PartThMethod(image, row, column, i, j, Thg))
					{
						image[i * column + j] = 255;
					}
					else
					{
						image[i * column + j] = 0;
					}
				}
				else
				{
					image[i * column + j] = 0;
				}
			}

		}
	}

	////输出
	//FILE *fp = fopen("Image.txt","w");
	////row = 3264;
	////column = 2448;
	//for(int i = 0; i < row; i++)
	//{
	//	for(int j = 0; j < column; j++)
	//	{
	//		fprintf(fp,"%d\t",image[i * column + j]);
	//	}
	//}
	//fclose(fp);
}


//Otsu全局阈值计算法
//image----图像
//row------行
//column---列
int OtsuMethod(int *image,int row,int column) 
{
	int MaxGray = 255;  //图像最大灰度值
	int WholeNum = row * column; //像素总数
	long int WholeGray = 0;
	//计算全局阈值Thg
	int Thg = 0;
	//计算灰度直方图
	int *ImageHist = new int[MaxGray + 1];
	memset(ImageHist,0,sizeof(ImageHist));
	for(int i = 0; i < row ; i++)
	{
		for(int j = 0; j <column; j++)
		{
			if(image[i * column + j] > MaxGray)
			{
				image[i * column + j] = MaxGray;				
			}
			else if(image[i * column + j] < 0)
			{
				image[i * column + j] = 0;
			}
			++ImageHist[image[i * column + j]];
			WholeGray += image[i * column + j];			
		}
	}
	//最大类间差分法（Otsu）
	int C0_GraySum = 0;
	int C1_GraySum = 0;
	int C0_NumSum = 0;
	int C1_NumSum = WholeNum - C0_NumSum;
	double max_otsu = -1;
	for(int th = 0 ;th < MaxGray; th ++)
	{
		C0_NumSum += ImageHist[th];
		C1_NumSum = WholeNum - C0_NumSum;

		C0_GraySum += ImageHist[th] * th;
		C1_GraySum = WholeGray - C0_GraySum;

		if(C0_NumSum == 0 || C1_NumSum == 0)
			continue;
		double u_0 = (double)C0_GraySum/(double)C0_NumSum;
		double u_1 = (double)C1_GraySum/(double)C1_NumSum;
		double w_0 = (double)C0_NumSum/(double)WholeNum;
		double w_1 = (double)C1_NumSum/(double)WholeNum;
		double otsu = w_0 * w_1 * (u_0 - u_1) * (u_0 - u_1);
		if(otsu > max_otsu)
		{
			max_otsu = otsu;
			Thg = th;
		}
	}

	delete []ImageHist;
	return Thg;
}

//局部阈值计算
//image----图像
//row------行
//column---列
//i0------待计算像素点行坐标
//j0------待计算像素点列坐标
//Thg-----全局阈值
int PartThMethod(int *Image, int row, int column, int i0, int j0,int Thg)
{
	//窗口大小 11*11
	int delta_row = 5;
	int delta_column = 5;

	//确定窗口在图像中的位置
	int StartRow = i0 - delta_row;
	int EndRow = i0 + delta_row + 1;
	int StartColumn = j0 - delta_column;
	int EndColumn = j0 + delta_column + 1;

	StartRow = StartRow < 0 ? 0 : StartRow;
	EndRow = EndRow > row  ? row : EndRow;
	StartColumn = StartColumn < 0 ? 0 : StartColumn;
	EndColumn = EndColumn > column ? column : EndColumn;

	//计算局部平均灰度
	int GraySum = 0;
	int WholeNum = (EndRow - StartRow) * (EndColumn - StartColumn);
	for(int i = StartRow; i < EndRow; i++)
	{
		for(int j = StartColumn; j < EndColumn; j++)
		{
			GraySum += Image[i * column + j];
		}
	}
	int GrayAverage = GraySum / WholeNum;

	//计算局部灰度标准差
	int SquareSum = 0;
	for(int i = StartRow; i < EndRow; i++)
	{
		for(int j = StartColumn; j < EndColumn; j++)
		{
			SquareSum += (Image[i * column + j] - GrayAverage) * (Image[i * column + j] - GrayAverage);
		}
	}
	int SquareAverage = SquareSum / WholeNum;
	double StandardDeviation = sqrt(double(SquareAverage));

	double Thl = 0;
	if(fabs(double(Image[i0 * column + j0] - GrayAverage)) < Thg / 16.0)
	{
		Thl = GrayAverage * (1.0 + 0.5 * (StandardDeviation / Thg - 1.0));
	}
	else
	{
		Thl = GrayAverage - StandardDeviation / Thg;
	}

	return (int)Thl;
}

//四分之一阈值计算
//image----图像
//row------行
//column---列
//i0------待计算像素点行坐标
//j0------待计算像素点列坐标
//Thg-----全局阈值

int QuarterThMethod(int *Image, int row, int column, int i0, int j0,int Thg)
{
	//窗口大小 11*11
	int delta_row = 5;
	int delta_column = 5;

	//确定窗口在图像中的位置
	int StartRow = i0 - delta_row;
	int EndRow = i0 + delta_row + 1;
	int StartColumn = j0 - delta_column;
	int EndColumn = j0 + delta_column + 1;

	StartRow = StartRow < 0 ? 0 : StartRow;
	EndRow = EndRow > row  ? row : EndRow;
	StartColumn = StartColumn < 0 ? 0 : StartColumn;
	EndColumn = EndColumn > column ? column : EndColumn;

	////计算四分之一阈值	
	int WholeNum = (EndRow - StartRow) * (EndColumn - StartColumn);
	int *AllGray = new int[WholeNum];
	int temp_k = 0;
	for(int i = StartRow; i < EndRow; i++)
	{
		for(int j = StartColumn; j < EndColumn; j++)
		{
			AllGray[temp_k] = Image[i * column + j];
			++temp_k;
		}
	}

	//排序
	quickSort(AllGray, 0, WholeNum-1);
	int k = 0;
	for(k = 0; k < (WholeNum - 1);k++)
	{
		if(AllGray[k]<=Thg && AllGray[k +1]>=Thg)
			break;
	}

	int quarter_num = WholeNum / 4;

	k = k - quarter_num -1;

	if( k < 0 )
		k = 0;

	return AllGray[k];
}


//快速排序  从小到大升序排列
void quickSort(int s[], int l, int r)  
{  
    if (l< r)  
    {        
        int i = l, j = r, x = s[l];  
        while (i < j)  
        {  
            while(i < j && s[j]>= x) // 从右向左找第一个小于x的数  
                j--;   
            if(i < j)  
                s[i++] = s[j];  
            while(i < j && s[i]< x) // 从左向右找第一个大于等于x的数  
                i++;   
            if(i < j)  
                s[j--] = s[i];  
        }  
        s[i] = x;  
        quickSort(s, l, i - 1); // 递归调用  
        quickSort(s, i + 1, r);  
    }  
}  
