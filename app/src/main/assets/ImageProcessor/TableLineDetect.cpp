#include "stdafx.h"

static int LinePointNum = 0;

void TableLineDetect(int *image,int *Lineimage,int row,int column)
{
	//HorLineDetect(image,Lineimage,row,column);

	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < column; j++)
		{
			Lineimage[i * column + j] = 255;
		}
	}

	AllLineDetect(image,Lineimage,row,column);
}

void HorLineDetect2(int *image,int *Lineimage,int row,int column)
{
	//搜索所有横向连接的线段
	std::vector<LengthRow>  AllHorLine;

	for(int i = 0 ;i < row; i++)
	{

	}
}


void HorLineDetect(int *image,int *Lineimage,int row,int column)
{
	//水平投影法获取 水平框线位置
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
	//计算平均值
	long int WholeLength = 0;
	for(int i = 0; i < row; i++)
	{
		WholeLength += HorProject[i].Length;
	}
	int Average = WholeLength / row + 1;
	//将低于平均值的投影置零
	for(int i = 0; i < row; i++)
	{
		if(HorProject[i].Length < Average)
		{
			HorProject[i].Length = 0;
		}
	}
	//寻找局部最高值
	for(int i = 0; i < row; i++)
	{
		int start = (i - 15) > 0 ? (i - 20) : 0;
		int end = (i + 15) < row ? (i + 20) : row;
		for(int j = start; j < i; j++)
		{
			if(HorProject[i].Length <= HorProject[j].Length)
			{
				HorProject[i].Length = 0;
				break;
			}
		}
		for(int j = i + 1; j < end; j++)
		{
			if(HorProject[i].Length <= HorProject[j].Length)
			{
				HorProject[i].Length = 0;
				break;
			}
		}
 	}
	//清除两个峰值中的低值
	LengthRow leftPeak,rightPeak,centerBottom;
	bool FindedLeft = false,FindedBottom = false;
	for(int i = 0; i < row; i++)
	{
		if(HorProject[i].Length != 0)
		{
			if(!FindedLeft)
			{
				leftPeak = HorProject[i] ;
				FindedLeft = true; 
			}
			else
			{
				if(!FindedBottom)
				{
					if(HorProject[i].Length > (leftPeak.Length * 2 / 3))
					{
						leftPeak = HorProject[i] ;
					}
					else
					{
						centerBottom = HorProject[i];
						FindedBottom = true;
					}
				}
				else
				{
					if(HorProject[i].Length < (centerBottom.Length * 2 / 3))
					{
						leftPeak = HorProject[i] ;
						FindedBottom = false;
					}
					else
					{
						HorProject[centerBottom.Row].Length = 0;
						leftPeak = HorProject[i] ;
						FindedBottom = false;
					}
				}
			}
		}
	}
	//排序
	//LengthquickSort(HorProject, 0, row);	
	
	//获取超过均值

	FILE *fp = fopen("HorProjection2.txt","w");
	for(int i = 0; i < row; i++)
	{
		fprintf(fp,"%d\t",HorProject[i].Length);
		fprintf(fp,"%d\n",HorProject[i].Row);
	}
	fclose(fp);

	int xxx = 1;
}


//按长度快速排序  从小到大升序排列
void LengthquickSort(LengthRow s[], int l, int r)  
{  
    if (l< r)  
    {        
        int i = l, j = r;
		LengthRow x = s[l];  
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



void AllLineDetect(int *image,int *Lineimage,int row,int column)
{
	//水平投影法获取 水平框线位置
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
    LengthRow MaxLength;
	MaxLength.Length = 0;
	for(int i = 0; i < row; i++)
	{
		if(HorProject[i].Length > MaxLength.Length)
		{
			MaxLength = HorProject[i];
		}
	}	

	//在Image中第MaxLength.Row行寻找表格线的一部分
	int StartColumn = 0;
	for(int j = 0; j < column; j++)
	{
		if(image[MaxLength.Row * column + j] == 0)
		{			
			FindStartPointEightNeibourhood(image,Lineimage,row,column,MaxLength.Row,j);
			if(LinePointNum >=100)
			{
				StartColumn = j;
				break;
			}
			else
			{
				LinePointNum = 0;
			}
		}	
	}
	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < column; j++)
		{
			Lineimage[i * column + j] = 255;
		}
	}
	//FindAllLineEightNeibourhood(image,Lineimage,row,column,MaxLength.Row,StartColumn,MaxLength.Row);
	//BFS(image,Lineimage,row,column,MaxLength.Row,StartColumn);
	BFS_delta(image,Lineimage,row,column,MaxLength.Row,StartColumn,2);
}

void FindStartPointEightNeibourhood(int *image,int *Lineimage,int row,int column,int i,int j)
{
    if( image[i * column + j]==0 && i>= 0&& j>=0 && i<row && column && Lineimage[i * column + j]== 255 && LinePointNum < 100)
    {
        ++LinePointNum;
        FindStartPointEightNeibourhood(image,Lineimage,row,column,i - 1,j - 1);
        FindStartPointEightNeibourhood(image,Lineimage,row,column,i - 1,j);
        FindStartPointEightNeibourhood(image,Lineimage,row,column,i - 1,j + 1);
        FindStartPointEightNeibourhood(image,Lineimage,row,column,i,j - 1);
		FindStartPointEightNeibourhood(image,Lineimage,row,column,i,j + 1);
		FindStartPointEightNeibourhood(image,Lineimage,row,column,i + 1,j - 1);
		FindStartPointEightNeibourhood(image,Lineimage,row,column,i + 1,j);
		FindStartPointEightNeibourhood(image,Lineimage,row,column,i + 1,j + 1);
    }
    return;
}


void FindAllLineEightNeibourhood(int *image,int *Lineimage,int row,int column,int i,int j,int StartedRow)
{
    if( image[i * column + j]==0 && i>= 0&& j>=0 && i<row && column && Lineimage[i * column + j]== 255 && abs(i - StartedRow) < 5)
    {
        Lineimage[i * column + j]= 0;
        FindAllLineEightNeibourhood(image,Lineimage,row,column,i - 1,j - 1,StartedRow);
        FindAllLineEightNeibourhood(image,Lineimage,row,column,i - 1,j,StartedRow);
        FindAllLineEightNeibourhood(image,Lineimage,row,column,i - 1,j + 1,StartedRow);
        FindAllLineEightNeibourhood(image,Lineimage,row,column,i,j - 1,StartedRow);
		FindAllLineEightNeibourhood(image,Lineimage,row,column,i,j + 1,StartedRow);
		FindAllLineEightNeibourhood(image,Lineimage,row,column,i + 1,j - 1,StartedRow);
		FindAllLineEightNeibourhood(image,Lineimage,row,column,i + 1,j,StartedRow);
		FindAllLineEightNeibourhood(image,Lineimage,row,column,i + 1,j + 1,StartedRow);
    }
    return;

}

void BFS(int *image,int *Lineimage,int row,int column, int i, int j)
{
	std::queue<std::vector<int>> q;
	int temp_xx[] ={j, i};
	std::vector<int> temp_1(temp_xx, temp_xx + 2);
	q.push(temp_1);
	Lineimage[i * column + j] = 0;

	while(!q.empty())
	{
		j = q.front()[0], i = q.front()[1];
		q.pop();
         
		//同一行
		if(j > 0 && image[j - 1 + i* column] == 0 && Lineimage[j - 1 + i* column] == 255)
		{
			int temp[] ={j - 1, i};
			std::vector<int> temp_x(temp, temp + 2);
			q.push(temp_x);
			Lineimage[j - 1 + i* column] = 0;
		}
		if(j < (column - 1) && image[j + 1 + i* column] == 0 && Lineimage[j + 1 + i* column] == 255)
		{
			int temp[] ={j + 1, i};
			std::vector<int> temp_x(temp, temp + 2);
			q.push(temp_x);
			Lineimage[j + 1 + i* column] = 0;
		}
		//上一行
		if(i > 0 && j > 0 && image[(j - 1) + (i - 1)* column] == 0 && Lineimage[(j - 1) + (i - 1)* column] == 255)
		{
			int temp[] ={j - 1, i - 1};
			std::vector<int> temp_x(temp, temp + 2);
			q.push(temp_x);
			Lineimage[(j - 1) + (i - 1)* column] = 0;
		}
		if(i > 0 && image[j + (i - 1)* column] == 0 && Lineimage[j + (i - 1)* column] == 255)
		{
			int temp[] ={j, i - 1};
			std::vector<int> temp_x(temp, temp + 2);
			q.push(temp_x);
			Lineimage[j + (i - 1)* column] = 0;
		}
		if(i > 0 && j < column - 1 && image[(j + 1) + (i - 1)* column] == 0 && Lineimage[(j + 1) + (i - 1)* column] == 255)
		{
			int temp[] ={j + 1, i - 1};
			std::vector<int> temp_x(temp, temp + 2);
			q.push(temp_x);
			Lineimage[(j + 1) + (i - 1)* column] = 0;
		}
		//下一行
		if(i < (row - 1) && j > 0 && image[(j - 1) + (i + 1)* column] == 0 && Lineimage[(j - 1) + (i + 1)* column] == 255)
		{
			int temp[] ={j - 1, i + 1};
			std::vector<int> temp_x(temp, temp + 2);
			q.push(temp_x);
			Lineimage[(j - 1) + (i + 1)* column] = 0;
		}
		if(i < (row - 1) && image[j + (i + 1)* column] == 0 && Lineimage[j + (i + 1)* column] == 255)
		{
			int temp[] ={j, i + 1};
			std::vector<int> temp_x(temp, temp + 2);
			q.push(temp_x);
			Lineimage[j + (i + 1)* column] = 0;
		}
		if(i < (row - 1) && j < (column - 1) && image[(j + 1) + (i + 1)* column] == 0 && Lineimage[(j + 1) + (i + 1)* column] == 255)
		{
			int temp[] ={j + 1, i + 1};
			std::vector<int> temp_x(temp, temp + 2);
			q.push(temp_x);
			Lineimage[(j + 1) + (i + 1)* column] = 0;
		}		
	}
	 
}

void BFS_delta(int *image,int *Lineimage,int row,int column, int i, int j,int delta)
{
	std::queue<std::vector<int>> q;
	int temp_xx[] ={j, i};
	std::vector<int> temp_1(temp_xx, temp_xx + 2);
	q.push(temp_1);
	Lineimage[i * column + j] = 0;

	while(!q.empty())
	{
		j = q.front()[0], i = q.front()[1];
		q.pop();
         
		int top = (i - delta) > 0 ? i - delta: 0;
		int bottom = (i + delta) < row ?  i + delta : row - 1;
		int left = (j - delta) > 0 ? j - delta: 0;
		int right = (j + delta) < column ?  j + delta : column - 1;
		

		for(int ii = top; ii <= bottom ;ii++)
		{
			for(int jj = left; jj <= right; jj++)
			{
				if(ii == i && jj ==j)
					continue;

				if(image[jj + ii* column] == 0 && Lineimage[jj + ii* column] == 255)
				{
					int temp[] ={jj, ii};
					std::vector<int> temp_x(temp, temp + 2);
					q.push(temp_x);
					Lineimage[jj + ii* column] = 0;
				}
			}
		}		
	}
	 
}
