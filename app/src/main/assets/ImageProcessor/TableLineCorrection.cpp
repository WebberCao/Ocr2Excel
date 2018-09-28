
#include "stdafx.h"

static int erase_table[]={
        0,0,1,1,0,0,1,1, 1,1,0,1,1,1,0,1,
        1,1,0,0,1,1,1,1, 0,0,0,0,0,0,0,1,
        0,0,1,1,0,0,1,1, 1,1,0,1,1,1,0,1,
        1,1,0,0,1,1,1,1, 0,0,0,0,0,0,0,1,
        1,1,0,0,1,1,0,0, 0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
        1,1,0,0,1,1,0,0, 1,1,0,1,1,1,0,1,
        0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
        0,0,1,1,0,0,1,1, 1,1,0,1,1,1,0,1,
        1,1,0,0,1,1,1,1, 0,0,0,0,0,0,0,1,
        0,0,1,1,0,0,1,1, 1,1,0,1,1,1,0,1,
        1,1,0,0,1,1,1,1, 0,0,0,0,0,0,0,0,
        1,1,0,0,1,1,0,0, 0,0,0,0,0,0,0,0,
        1,1,0,0,1,1,1,1, 0,0,0,0,0,0,0,0,
        1,1,0,0,1,1,0,0, 1,1,0,1,1,1,0,0,
        1,1,0,0,1,1,1,0, 1,1,0,0,1,0,0,0};

static int PowTwo_table[]={1,2,4,8,16,32,64,128};

void TableLineCorrection(int *image,int row,int column,int RowLineDistance,int ColumnLineDistance,int CombinedDeltaRowLine,int CombinedDeltaColumnLine,std::vector<HorLine> &AllHorLine,std::vector<VerLine> &AllVerLine)
{
	//获取水平表格线和竖直表格线
	/*std::vector<HorLine> AllHorLine = HorLineExtract(image,row,column);
	std::vector<VerLine> AllVerLine = VerLineExtract(image,row,column);*/
	AllHorLine = PreciseHorLineExtract(image,row,column,CombinedDeltaRowLine);
	AllVerLine = PreciseVerLineExtract(image,row,column,CombinedDeltaColumnLine);

	//调整水平表格线和竖直表格线的位置
	for(int i = 0; i < AllHorLine.size();i++)
	{
		for(int j = 0;j < AllVerLine.size();j++)
		{
			if(abs(AllHorLine[i].StartColumn - AllVerLine[j].NthColumn) < ColumnLineDistance)
			{
				AllHorLine[i].StartColumn = AllVerLine[j].NthColumn;
			}
			if(abs(AllHorLine[i].EndColumn - AllVerLine[j].NthColumn) < ColumnLineDistance)
			{
				AllHorLine[i].EndColumn = AllVerLine[j].NthColumn;
			}
		}
	}

	for(int i = 0; i < AllVerLine.size();i++)
	{
		for(int j = 0;j < AllHorLine.size();j++)
		{
			if(abs(AllVerLine[i].StartRow - AllHorLine[j].NthRow) < RowLineDistance)
			{
				AllVerLine[i].StartRow = AllHorLine[j].NthRow;
			}
			if(abs(AllVerLine[i].EndRow - AllHorLine[j].NthRow) < RowLineDistance)
			{
				AllVerLine[i].EndRow = AllHorLine[j].NthRow;
			}
		}
	}


	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < column; j++)
		{
			image[i * column + j] = 255;
		}
	}

	for(int i = 0; i < AllHorLine.size();i++)
	{
		for(int m = AllHorLine[i].StartColumn; m <= AllHorLine[i].EndColumn;m++ )
		{
			image[AllHorLine[i].NthRow * column + m] = 0;
		}
	}

	for(int i = 0; i < AllVerLine.size();i++)
	{
		for(int m = AllVerLine[i].StartRow; m <= AllVerLine[i].EndRow;m++ )
		{
			image[AllVerLine[i].NthColumn + m * column] = 0;
		}
	}


	//赋值查看效果
	/*for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < column; j++)
		{
			image[i * column + j] = tempImage[i * column + j];
		}
	}*/

}

void  ThiningTableLine(int *image,int row,int column) 
{
	
	for(int i = 1; i < (row - 1) ; i++)
	{
		bool Skipped = false;
		for(int j = 1; j < (column - 1); j++)
		{			
			if(Skipped)
			{
				Skipped = false;
				continue;
			}
			if(image[i * column + j] == 255)
			{
				continue;
			}
			if(image[i * column + j - 1] == 0 && image[i * column + j + 1] == 0)
			{
				;
			}
			else
			{
				int index = PowTwo_table[0] * (image[(i - 1) * column + j - 1] / 255) + 
							PowTwo_table[1] * (image[(i - 1) * column + j ] / 255) +
							PowTwo_table[2] * (image[(i - 1) * column + j + 1] / 255) +
							PowTwo_table[3] * (image[i * column + j - 1] / 255) +
							PowTwo_table[4] * (image[i * column + j + 1] / 255) +
							PowTwo_table[5] * (image[(i + 1) * column + j - 1] / 255) +
							PowTwo_table[6] * (image[(i + 1) * column + j ] / 255) +
							PowTwo_table[7] * (image[(i + 1) * column + j + 1] / 255);
				if(erase_table[index] == 1)
				{
					image[i * column + j] = 255;
					Skipped = true;
				}
			}
		}
	}


	
	for(int i = 1; i < (row - 1) ; i++)
	{
		bool Skipped = false;
		for(int j = 1; j < (column - 1); j++)
		{			
			if(Skipped)
			{
				Skipped = false;
				continue;
			}
			if(image[i * column + j] == 255)
			{
				continue;
			}
			if(image[(i - 1) * column + j] == 0 && image[(i + 1) * column + j] == 0)
			{
				;
			}
			else
			{
				int index = PowTwo_table[0] * (image[(i - 1) * column + j - 1] / 255) + 
							PowTwo_table[1] * (image[(i - 1) * column + j ] / 255) +
							PowTwo_table[2] * (image[(i - 1) * column + j + 1] / 255) +
							PowTwo_table[3] * (image[i * column + j - 1] / 255) +
							PowTwo_table[4] * (image[i * column + j + 1] / 255) +
							PowTwo_table[5] * (image[(i + 1) * column + j - 1] / 255) +
							PowTwo_table[6] * (image[(i + 1) * column + j ] / 255) +
							PowTwo_table[7] * (image[(i + 1) * column + j + 1] / 255);
				if(erase_table[index] == 1)
				{
					image[i * column + j] = 255;
					Skipped = true;
				}
			}
		}
	}
}

std::vector<HorLine> HorLineExtract(int *image,int row,int column,int CombinedDeltaLine)
{
	/*int DeltaRowLine = column / 60;
	if(DeltaRowLine < 5) 
		DeltaRowLine = 5;

	int DeltaColumnLine = row / 45;
	if(DeltaColumnLine < 10) 
		DeltaColumnLine = 10;*/

	int *tempImage = new int[row * column];
	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < column; j++)
		{	
			tempImage[i * column + j] = 255;
		}
	}

	//水平线提取
	for(int i = 0; i < row ; i++)
	{
		for(int j = 0; j < (column - 1); j++)
		{
			if(image[i * column + j] == 0 && image[i * column + j + 1] == 0)
			{
				tempImage[i * column + j] = 0;
				tempImage[i * column + j + 1] = 0;
			}
		}
	}
	//水平线拉直
	for(int i = 1; i < (row - 1); i++)
	{
		for(int j = 1; j < (column - 1); j++)
		{
			if(image[i * column + j] == 0 && (image[(i - 1) * column + j + 1] == 0 
				|| image[i * column + j + 1] == 0 ||image[(i + 1) * column + j + 1] == 0))
			{
				tempImage[i * column + j] = 0;
				tempImage[i * column + j + 1] = 0;

				tempImage[(i - 1) * column + j + 1] = 255;
				tempImage[(i + 1) * column + j + 1] = 255;
			}
		}
	}
	//断裂线合并
	//获取所有水平线起始列号，终止列号，行号
	bool FindLine = false;
	std::vector<HorLine> AllHorLine;
	for(int i = 1; i < (row - 1); i++)
	{
		for(int j = 1; j < (column - 1); j++)
		{
			if(!FindLine)
			{
				if((tempImage[i * column + j] == 0) && tempImage[i * column + j + 1] == 0)
				{
					FindLine = true;
				    HorLine tempHorLine;
					tempHorLine.NthRow = i;
					tempHorLine.StartColumn = j;
					AllHorLine.push_back(tempHorLine);
				}
			}
			else
			{
				if((tempImage[i * column + j] == 0) && tempImage[i * column + j + 1] == 255)
				{
					FindLine = false;
					AllHorLine[AllHorLine.size() - 1].EndColumn = j;
				}
			}
		}
	}
	//水平断裂线合并
	bool IsCombined = false;
	for(int i = 0; i < AllHorLine.size();)
	{
		IsCombined = false;
		for(int j = i + 1; j< AllHorLine.size();)
		{
			if((AllHorLine[j].NthRow - AllHorLine[i].NthRow) < 20)
			{
				//合并
				int temp1 = (AllHorLine[j].StartColumn - AllHorLine[i].EndColumn);
				
				if(temp1 >= 0 && temp1 <= 40)
				{
					AllHorLine[i].NthRow = (AllHorLine[j].NthRow + AllHorLine[i].NthRow) / 2;
					AllHorLine[i].StartColumn = AllHorLine[i].StartColumn;
					AllHorLine[i].EndColumn = AllHorLine[j].EndColumn;

					//删除第j条线
					AllHorLine.erase(AllHorLine.begin() + j);
					IsCombined = true;
					break;
				}
				else 
				{
					int temp2 = (AllHorLine[i].StartColumn - AllHorLine[j].EndColumn);
					if(temp2 >= 0 && temp2 <= 40)
					{
						
						AllHorLine[i].NthRow = (AllHorLine[j].NthRow + AllHorLine[i].NthRow) / 2;
						AllHorLine[i].StartColumn = AllHorLine[j].StartColumn;
						AllHorLine[i].EndColumn = AllHorLine[i].EndColumn;

						//删除第j条线
						AllHorLine.erase(AllHorLine.begin() + j);
						IsCombined = true;
						break;
					}
				}
				++j;				
			}
			else
			{
				IsCombined = false;
				break;
			}
		}
		if(!IsCombined)
		{
			++i;
		}
	}
	//删除短线段
	for(int i = 0; i < AllHorLine.size();)
	{
		if((AllHorLine[i].EndColumn - AllHorLine[i].StartColumn) < 30)
		{
			AllHorLine.erase(AllHorLine.begin() + i);
			continue;
		}
		++i;
	}

	//合并重叠线
	for(int i = 0; i < AllHorLine.size();)
	{
		IsCombined = false;
		for(int j = i + 1; j< AllHorLine.size();)
		{
			if((AllHorLine[j].NthRow - AllHorLine[i].NthRow) < CombinedDeltaLine)
			{
				//合并重叠部分
				int temp1 = AllHorLine[i].StartColumn < AllHorLine[j].StartColumn ? AllHorLine[j].StartColumn :AllHorLine[i].StartColumn; //取较大值
				int temp2 =  AllHorLine[i].EndColumn > AllHorLine[j].EndColumn ? AllHorLine[j].EndColumn :AllHorLine[i].EndColumn; //取较小值
				if(temp1 <= temp2 ) //第j条线与第i条线有交集
				{
					AllHorLine[i].StartColumn = AllHorLine[i].StartColumn > AllHorLine[j].StartColumn ? AllHorLine[j].StartColumn :AllHorLine[i].StartColumn; //取较小值
					AllHorLine[i].EndColumn = AllHorLine[i].EndColumn < AllHorLine[j].EndColumn ? AllHorLine[j].EndColumn :AllHorLine[i].EndColumn; //取较大值
				
					//删除第j条线
					AllHorLine.erase(AllHorLine.begin() + j);
					IsCombined = true;
					break;
				}				
				++j;
			}
			else
			{
				IsCombined = false;
				break;
			}
		}
		if(!IsCombined)
		{
			++i;
		}
	}

	
	delete []tempImage;

	return AllHorLine;
}


std::vector<VerLine> VerLineExtract(int *image,int row,int column,int CombinedDeltaLine)
{
	/*int DeltaRowLine = column / 45;
	if(DeltaRowLine < 10) 
		DeltaRowLine = 10;

	int DeltaColumnLine = row / 60;
	if(DeltaColumnLine < 5) 
		DeltaColumnLine = 5;*/
	int *tempImage = new int[row * column];
	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < column; j++)
		{	
			tempImage[i * column + j] = 255;
		}
	}

	//竖直线提取	
	for(int j = 0; j < column ; j++)
	{
		for(int i = 0; i < (row - 1) ; i++)
		{
			if(image[i * column + j] == 0 && image[(i + 1) * column + j] == 0)
			{
				tempImage[i * column + j] = 0;
				tempImage[(i + 1) * column + j] = 0;
			}
		}
	}//*/
	//竖直线拉直
	
	for(int j = 1; j < (column - 1); j++)
	{
		for(int i = 1; i < (row - 1); i++)
		{
			if(tempImage[i * column + j] == 0 && (tempImage[(i + 1) * column + j - 1] == 0 
				|| tempImage[(i + 1) * column + j] == 0 ||tempImage[(i + 1) * column + j + 1] == 0))
			{
				tempImage[i * column + j] = 0;
				tempImage[(i + 1) * column + j] = 0;

				tempImage[(i + 1) * column + j - 1] = 255;
				tempImage[(i + 1) * column + j + 1] = 255;
			}
		}
	}

	//断裂线合并
	//获取所有水平线起始列号，终止列号，行号
	bool FindLine = false;
	std::vector<VerLine> AllVerLine;
	
	for(int j = 1; j < (column - 1); j++)
	{
		for(int i = 1; i < (row - 1); i++)
		{
			if(!FindLine)
			{
				if((tempImage[i * column + j] == 0) && tempImage[(i + 1) * column + j] == 0)
				{
					FindLine = true;
				    VerLine tempVerLine;
					tempVerLine.NthColumn = j;
					tempVerLine.StartRow = i;
					AllVerLine.push_back(tempVerLine);
				}
			}
			else
			{
				if((tempImage[i * column + j] == 0) && tempImage[(i + 1) * column + j] == 255)
				{
					FindLine = false;
					AllVerLine[AllVerLine.size() - 1].EndRow = i;
				}
			}
		}
	}
	//水平断裂线合并
	bool IsCombined = false;
	for(int i = 0; i < AllVerLine.size();)
	{
		IsCombined = false;
		for(int j = i + 1; j< AllVerLine.size();)
		{
			if((AllVerLine[j].NthColumn - AllVerLine[i].NthColumn) < 20)
			{
				//合并
				int temp1 = (AllVerLine[j].StartRow - AllVerLine[i].EndRow);
				
				if(temp1 >= 0 && temp1 <= 40)
				{
					AllVerLine[i].NthColumn = (AllVerLine[j].NthColumn + AllVerLine[i].NthColumn) / 2;
					AllVerLine[i].StartRow = AllVerLine[i].StartRow;
					AllVerLine[i].EndRow = AllVerLine[j].EndRow;

					//删除第j条线
					AllVerLine.erase(AllVerLine.begin() + j);
					IsCombined = true;
					break;
				}
				else 
				{
					int temp2 = (AllVerLine[i].StartRow - AllVerLine[j].EndRow);
					if(temp2 >= 0 && temp2 <= 40)
					{
						
						AllVerLine[i].NthColumn = (AllVerLine[j].NthColumn + AllVerLine[i].NthColumn) / 2;
						AllVerLine[i].StartRow = AllVerLine[j].StartRow;
						AllVerLine[i].EndRow = AllVerLine[i].EndRow;

						//删除第j条线
						AllVerLine.erase(AllVerLine.begin() + j);
						IsCombined = true;
						break;
					}
				}
				++j;				
			}
			else
			{
				IsCombined = false;
				break;
			}
		}
		if(!IsCombined)
		{
			++i;
		}
	}
	//删除短线段
	for(int i = 0; i < AllVerLine.size();)
	{
		if((AllVerLine[i].EndRow - AllVerLine[i].StartRow) < 30)
		{
			AllVerLine.erase(AllVerLine.begin() + i);
			continue;
		}
		++i;
	}

	//合并重叠线
	for(int i = 0; i < AllVerLine.size();)
	{
		IsCombined = false;
		for(int j = i + 1; j< AllVerLine.size();)
		{
			if((AllVerLine[j].NthColumn- AllVerLine[i].NthColumn) < CombinedDeltaLine)
			{
				//合并重叠部分
				int temp1 = AllVerLine[i].StartRow < AllVerLine[j].StartRow ? AllVerLine[j].StartRow :AllVerLine[i].StartRow; //取较大值
				int temp2 =  AllVerLine[i].EndRow > AllVerLine[j].EndRow ? AllVerLine[j].EndRow :AllVerLine[i].EndRow; //取较小值
				if(temp1 <= temp2 ) //第j条线与第i条线有交集
				{
					AllVerLine[i].StartRow = AllVerLine[i].StartRow > AllVerLine[j].StartRow ? AllVerLine[j].StartRow :AllVerLine[i].StartRow; //取较小值
					AllVerLine[i].EndRow = AllVerLine[i].EndRow < AllVerLine[j].EndRow ? AllVerLine[j].EndRow :AllVerLine[i].EndRow; //取较大值
				
					//删除第j条线
					AllVerLine.erase(AllVerLine.begin() + j);
					IsCombined = true;
					break;
				}	
				////删除被包含的线
				//int temp1 = (AllVerLine[i].StartRow - AllVerLine[j].StartRow);
				//int temp2 = (AllVerLine[i].EndRow - AllVerLine[j].EndRow);
				//if(temp1 <= 0 && temp2 >= 0) //第j条线被包含在第i条线内
				//{
				//	//删除第j条线
				//	AllVerLine.erase(AllVerLine.begin() + j);
				//	IsCombined = true;
				//	break;
				//}
				//else
				//{
				//	int temp3 = (AllVerLine[j].StartRow - AllVerLine[i].StartRow);
				//	int temp4 = (AllVerLine[j].EndRow - AllVerLine[i].EndRow);
				//	if(temp3 <= 0 && temp4 >= 0) //第i条线被包含在第j条线内
				//	{
				//		//将第j条线的值赋给第i条线
				//		AllVerLine[i] = AllVerLine[j];
				//		//删除第j条线
				//		AllVerLine.erase(AllVerLine.begin() + j);
				//		IsCombined = true;
				//		break;
				//	}
				//}
				++j;
			}
			else
			{
				IsCombined = false;
				break;
			}
		}
		if(!IsCombined)
		{
			++i;
		}
	}

	
	delete []tempImage;

	return AllVerLine;
}



std::vector<HorLine> PreciseHorLineExtract(int *image,int row,int column,int CombinedDeltaLine)
{
	/*int DeltaRowLine = column / 60;
	if(DeltaRowLine < 5) 
		DeltaRowLine = 5;

	int DeltaColumnLine = row / 45;
	if(DeltaColumnLine < 10) 
		DeltaColumnLine = 10;*/

	int *tempImage = new int[row * column];
	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < column; j++)
		{	
			tempImage[i * column + j] = 255;
		}
	}

	//水平线提取
	for(int i = 0; i < row ; i++)
	{
		for(int j = 0; j < (column - 1); j++)
		{
			if(image[i * column + j] == 0 && image[i * column + j + 1] == 0)
			{
				tempImage[i * column + j] = 0;
				tempImage[i * column + j + 1] = 0;
			}
		}
	}
	//水平线拉直
	for(int i = 1; i < (row - 1); i++)
	{
		for(int j = 1; j < (column - 1); j++)
		{
			if(image[i * column + j] == 0 && (image[(i - 1) * column + j + 1] == 0 
				|| image[i * column + j + 1] == 0 ||image[(i + 1) * column + j + 1] == 0))
			{
				tempImage[i * column + j] = 0;
				tempImage[i * column + j + 1] = 0;

				tempImage[(i - 1) * column + j + 1] = 255;
				tempImage[(i + 1) * column + j + 1] = 255;
			}
		}
	}
	//断裂线合并
	//获取所有水平线起始列号，终止列号，行号
	bool FindLine = false;
	std::vector<DoubleHorLine> AllHorLine;
	for(int i = 1; i < (row - 1); i++)
	{
		for(int j = 1; j < (column - 1); j++)
		{
			if(!FindLine)
			{
				if((tempImage[i * column + j] == 0) && tempImage[i * column + j + 1] == 0)
				{
					FindLine = true;
				    DoubleHorLine tempHorLine;
					tempHorLine.NthRow = i;
					tempHorLine.StartColumn = j;
					AllHorLine.push_back(tempHorLine);
				}
			}
			else
			{
				if((tempImage[i * column + j] == 0) && tempImage[i * column + j + 1] == 255)
				{
					FindLine = false;
					AllHorLine[AllHorLine.size() - 1].EndColumn = j;
				}
			}
		}
	}
	//水平断裂线合并
	bool IsCombined = false;
	for(int i = 0; i < AllHorLine.size();)
	{
		IsCombined = false;
		for(int j = i + 1; j< AllHorLine.size();)
		{
			if((AllHorLine[j].NthRow - AllHorLine[i].NthRow) < CombinedDeltaLine)
			{
				//合并
				int temp1 = (AllHorLine[j].StartColumn - AllHorLine[i].EndColumn);
				
				if(temp1 >= 0 && temp1 <= 40)
				{
					int length_i = AllHorLine[i].EndColumn - AllHorLine[i].StartColumn;
					int length_j = AllHorLine[j].EndColumn - AllHorLine[j].StartColumn;

					AllHorLine[i].NthRow = (AllHorLine[j].NthRow * length_j  + AllHorLine[i].NthRow * length_i) / (length_i + length_j);
					AllHorLine[i].StartColumn = AllHorLine[i].StartColumn;
					AllHorLine[i].EndColumn = AllHorLine[j].EndColumn;

					//删除第j条线
					AllHorLine.erase(AllHorLine.begin() + j);
					IsCombined = true;
					break;
				}
				else 
				{
					int temp2 = (AllHorLine[i].StartColumn - AllHorLine[j].EndColumn);
					if(temp2 >= 0 && temp2 <= 40)
					{						
						int length_i = AllHorLine[i].EndColumn - AllHorLine[i].StartColumn;
						int length_j = AllHorLine[j].EndColumn - AllHorLine[j].StartColumn;

						AllHorLine[i].NthRow = (AllHorLine[j].NthRow * length_j  + AllHorLine[i].NthRow * length_i) / (length_i + length_j);
					
						AllHorLine[i].StartColumn = AllHorLine[j].StartColumn;
						AllHorLine[i].EndColumn = AllHorLine[i].EndColumn;

						//删除第j条线
						AllHorLine.erase(AllHorLine.begin() + j);
						IsCombined = true;
						break;
					}
				}
				++j;				
			}
			else
			{
				IsCombined = false;
				break;
			}
		}
		if(!IsCombined)
		{
			++i;
		}
	}
	//删除短线段
	for(int i = 0; i < AllHorLine.size();)
	{
		if((AllHorLine[i].EndColumn - AllHorLine[i].StartColumn) < 30)
		{
			AllHorLine.erase(AllHorLine.begin() + i);
			continue;
		}
		++i;
	}

	//合并重叠线
	for(int i = 0; i < AllHorLine.size();)
	{
		IsCombined = false;
		for(int j = i + 1; j< AllHorLine.size();)
		{
			if((AllHorLine[j].NthRow - AllHorLine[i].NthRow) < CombinedDeltaLine)
			{
				//合并重叠部分
				int temp1 = AllHorLine[i].StartColumn < AllHorLine[j].StartColumn ? AllHorLine[j].StartColumn :AllHorLine[i].StartColumn; //取较大值
				int temp2 =  AllHorLine[i].EndColumn > AllHorLine[j].EndColumn ? AllHorLine[j].EndColumn :AllHorLine[i].EndColumn; //取较小值
				if(temp1 <= temp2 ) //第j条线与第i条线有交集
				{
					int length_i = AllHorLine[i].EndColumn - AllHorLine[i].StartColumn;
					int length_j = AllHorLine[j].EndColumn - AllHorLine[j].StartColumn;

					AllHorLine[i].NthRow = (AllHorLine[j].NthRow * length_j  + AllHorLine[i].NthRow * length_i) / (length_i + length_j);
					
					AllHorLine[i].StartColumn = AllHorLine[i].StartColumn > AllHorLine[j].StartColumn ? AllHorLine[j].StartColumn :AllHorLine[i].StartColumn; //取较小值
					AllHorLine[i].EndColumn = AllHorLine[i].EndColumn < AllHorLine[j].EndColumn ? AllHorLine[j].EndColumn :AllHorLine[i].EndColumn; //取较大值
				
					//删除第j条线
					AllHorLine.erase(AllHorLine.begin() + j);
					IsCombined = true;
					break;
				}				
				++j;
			}
			else
			{
				IsCombined = false;
				break;
			}
		}
		if(!IsCombined)
		{
			++i;
		}
	}

	
	delete []tempImage;

	std::vector<HorLine> AllIntHorLine;

	for(int i  = 0; i<AllHorLine.size();i++)
	{
		HorLine tempHorLine;
		tempHorLine.StartColumn = AllHorLine[i].StartColumn;
		tempHorLine.EndColumn = AllHorLine[i].EndColumn;
		tempHorLine.NthRow = (int)AllHorLine[i].NthRow;

		AllIntHorLine.push_back(tempHorLine);
	}

	return AllIntHorLine;
}

std::vector<VerLine> PreciseVerLineExtract(int *image,int row,int column,int CombinedDeltaLine)
{
	/*int DeltaRowLine = column / 45;
	if(DeltaRowLine < 10) 
		DeltaRowLine = 10;

	int DeltaColumnLine = row / 60;
	if(DeltaColumnLine < 5) 
		DeltaColumnLine = 5;*/
	int *tempImage = new int[row * column];
	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < column; j++)
		{	
			tempImage[i * column + j] = 255;
		}
	}

	//竖直线提取	
	for(int j = 0; j < column ; j++)
	{
		for(int i = 0; i < (row - 1) ; i++)
		{
			if(image[i * column + j] == 0 && image[(i + 1) * column + j] == 0)
			{
				tempImage[i * column + j] = 0;
				tempImage[(i + 1) * column + j] = 0;
			}
		}
	}//*/
	//竖直线拉直
	
	for(int j = 1; j < (column - 1); j++)
	{
		for(int i = 1; i < (row - 1); i++)
		{
			if(tempImage[i * column + j] == 0 && (tempImage[(i + 1) * column + j - 1] == 0 
				|| tempImage[(i + 1) * column + j] == 0 ||tempImage[(i + 1) * column + j + 1] == 0))
			{
				tempImage[i * column + j] = 0;
				tempImage[(i + 1) * column + j] = 0;

				tempImage[(i + 1) * column + j - 1] = 255;
				tempImage[(i + 1) * column + j + 1] = 255;
			}
		}
	}

	//断裂线合并
	//获取所有水平线起始列号，终止列号，行号
	bool FindLine = false;
	std::vector<DoubleVerLine> AllVerLine;
	
	for(int j = 1; j < (column - 1); j++)
	{
		for(int i = 1; i < (row - 1); i++)
		{
			if(!FindLine)
			{
				if((tempImage[i * column + j] == 0) && tempImage[(i + 1) * column + j] == 0)
				{
					FindLine = true;
				    DoubleVerLine tempVerLine;
					tempVerLine.NthColumn = j;
					tempVerLine.StartRow = i;
					AllVerLine.push_back(tempVerLine);
				}
			}
			else
			{
				if((tempImage[i * column + j] == 0) && tempImage[(i + 1) * column + j] == 255)
				{
					FindLine = false;
					AllVerLine[AllVerLine.size() - 1].EndRow = i;
				}
			}
		}
	}
	//水平断裂线合并
	bool IsCombined = false;
	for(int i = 0; i < AllVerLine.size();)
	{
		IsCombined = false;
		for(int j = i + 1; j< AllVerLine.size();)
		{
			if((AllVerLine[j].NthColumn - AllVerLine[i].NthColumn) < CombinedDeltaLine)
			{
				//合并
				int temp1 = (AllVerLine[j].StartRow - AllVerLine[i].EndRow);
				
				if(temp1 >= 0 && temp1 <= 40)
				{
					int length_i = AllVerLine[i].EndRow - AllVerLine[i].StartRow;
					int length_j = AllVerLine[j].EndRow - AllVerLine[j].StartRow;

					AllVerLine[i].NthColumn= (AllVerLine[j].NthColumn * length_j  + AllVerLine[i].NthColumn * length_i) / (length_i + length_j);
					
					AllVerLine[i].StartRow = AllVerLine[i].StartRow;
					AllVerLine[i].EndRow = AllVerLine[j].EndRow;

					//删除第j条线
					AllVerLine.erase(AllVerLine.begin() + j);
					IsCombined = true;
					break;
				}
				else 
				{
					int temp2 = (AllVerLine[i].StartRow - AllVerLine[j].EndRow);
					if(temp2 >= 0 && temp2 <= 40)
					{
						int length_i = AllVerLine[i].EndRow - AllVerLine[i].StartRow;
						int length_j = AllVerLine[j].EndRow - AllVerLine[j].StartRow;
						AllVerLine[i].NthColumn= (AllVerLine[j].NthColumn * length_j  + AllVerLine[i].NthColumn * length_i) / (length_i + length_j);
											
						AllVerLine[i].NthColumn = (AllVerLine[j].NthColumn + AllVerLine[i].NthColumn) / 2;
						AllVerLine[i].StartRow = AllVerLine[j].StartRow;
						AllVerLine[i].EndRow = AllVerLine[i].EndRow;

						//删除第j条线
						AllVerLine.erase(AllVerLine.begin() + j);
						IsCombined = true;
						break;
					}
				}
				++j;				
			}
			else
			{
				IsCombined = false;
				break;
			}
		}
		if(!IsCombined)
		{
			++i;
		}
	}
	//删除短线段
	for(int i = 0; i < AllVerLine.size();)
	{
		if((AllVerLine[i].EndRow - AllVerLine[i].StartRow) < 30)
		{
			AllVerLine.erase(AllVerLine.begin() + i);
			continue;
		}
		++i;
	}

	//合并重叠线
	for(int i = 0; i < AllVerLine.size();)
	{
		IsCombined = false;
		for(int j = i + 1; j< AllVerLine.size();)
		{
			if((AllVerLine[j].NthColumn- AllVerLine[i].NthColumn) < CombinedDeltaLine)
			{
				//合并重叠部分
				int temp1 = AllVerLine[i].StartRow < AllVerLine[j].StartRow ? AllVerLine[j].StartRow :AllVerLine[i].StartRow; //取较大值
				int temp2 =  AllVerLine[i].EndRow > AllVerLine[j].EndRow ? AllVerLine[j].EndRow :AllVerLine[i].EndRow; //取较小值
				if(temp1 <= temp2 ) //第j条线与第i条线有交集
				{
					int length_i = AllVerLine[i].EndRow - AllVerLine[i].StartRow;
					int length_j = AllVerLine[j].EndRow - AllVerLine[j].StartRow;

					AllVerLine[i].NthColumn= (AllVerLine[j].NthColumn * length_j  + AllVerLine[i].NthColumn * length_i) / (length_i + length_j);
					
					AllVerLine[i].StartRow = AllVerLine[i].StartRow > AllVerLine[j].StartRow ? AllVerLine[j].StartRow :AllVerLine[i].StartRow; //取较小值
					AllVerLine[i].EndRow = AllVerLine[i].EndRow < AllVerLine[j].EndRow ? AllVerLine[j].EndRow :AllVerLine[i].EndRow; //取较大值
				
					//删除第j条线
					AllVerLine.erase(AllVerLine.begin() + j);
					IsCombined = true;
					break;
				}				
				++j;
			}
			else
			{
				IsCombined = false;
				break;
			}
		}
		if(!IsCombined)
		{
			++i;
		}
	}

	
	delete []tempImage;


	std::vector<VerLine> AllIntVerLine;

	for(int i  = 0; i<AllVerLine.size();i++)
	{
		VerLine tempVerLine;
		tempVerLine.StartRow = AllVerLine[i].StartRow;
		tempVerLine.EndRow = AllVerLine[i].EndRow;
		tempVerLine.NthColumn = (int)AllVerLine[i].NthColumn;

		AllIntVerLine.push_back(tempVerLine);
	}


	return AllIntVerLine;
}
