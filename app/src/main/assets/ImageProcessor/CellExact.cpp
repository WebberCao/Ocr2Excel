#include "stdafx.h"



void CellExact(std::vector<HorLine> AllHorLine, std::vector<VerLine> AllVerLine,TrimCell *CellResult)
{
	
	std::vector<std::vector<TablePoint>> inersection_point;

	//将所有横线、竖线排序，得到其行号，列号
	std::map<int,int> MapHorLineNum;
	std::map<int,int> MapVerLineNum;

	for(int i = 0; i < AllHorLine.size();i++)
	{
		MapHorLineNum[AllHorLine[i].NthRow] = 1;
	}
	for(int i = 0; i < AllVerLine.size();i++)
	{
		MapVerLineNum[AllVerLine[i].NthColumn] = 1;
	}

	std::map <int, int>::iterator m1_Iter;
	int temp_m = 0;
	for ( m1_Iter = MapHorLineNum.begin(); m1_Iter != MapHorLineNum.end( ); m1_Iter++ ) //map元素自动按KEY值升序排列    
	{
		m1_Iter->second = temp_m;
		temp_m++;
	}
	
	temp_m = 0;
	for ( m1_Iter = MapVerLineNum.begin(); m1_Iter != MapVerLineNum.end( ); m1_Iter++ ) //map元素自动按KEY值升序排列    
	{
		m1_Iter->second = temp_m;
		temp_m++;
	}

	//计算交点
	PointPos HorPos,VerPos;
	for(int i = 0; i < AllHorLine.size();i++)
	{
		std::vector<TablePoint> temp;
		for(int j = 0;j < AllVerLine.size();j++)
		{						
			if(AllVerLine[j].NthColumn>=AllHorLine[i].StartColumn && AllVerLine[j].NthColumn <=AllHorLine[i].EndColumn)			
			{
				if(AllHorLine[i].NthRow >=AllVerLine[j].StartRow && AllHorLine[i].NthRow <=AllVerLine[j].EndRow)
				{					
					TablePoint temp2;
					temp2.Column = AllVerLine[j].NthColumn;
					temp2.Row = AllHorLine[i].NthRow;
					temp2.Searched = false;
					//判断交点在水平线的位置
					if(temp2.Column == AllHorLine[i].StartColumn)
					{
						HorPos = PointPos::start;
					}
					else if(temp2.Column == AllHorLine[i].EndColumn)
					{
						HorPos = PointPos::end;
					}
					else
					{
						HorPos = PointPos::center;
					}
					//判断交点在竖直线的位置
					if(temp2.Row == AllVerLine[j].StartRow)
					{
						VerPos = PointPos::start;
					}
					else if(temp2.Row == AllVerLine[j].EndRow)
					{
						VerPos = PointPos::end;
					}
					else
					{
						VerPos = PointPos::center;
					}

					//判断交点类型
					if(HorPos == PointPos::start && VerPos == PointPos::end)
					{
						temp2.type = 1;
					}
					else if(HorPos == PointPos::center && VerPos == PointPos::end)
					{
						temp2.type = 2;
					}
					else if(HorPos == PointPos::end && VerPos == PointPos::end)
					{
						temp2.type = 3;
					}
					else if(HorPos == PointPos::start && VerPos == PointPos::center)
					{
						temp2.type = 4;
					}
					else if(HorPos == PointPos::center && VerPos == PointPos::center)
					{
						temp2.type = 5;
					}
					else if(HorPos == PointPos::end && VerPos == PointPos::center)
					{
						temp2.type = 6;
					}
					else if(HorPos == PointPos::start && VerPos == PointPos::start)
					{
						temp2.type = 7;
					}
					else if(HorPos == PointPos::center && VerPos == PointPos::start)
					{
						temp2.type = 8;
					}
					else if(HorPos == PointPos::end && VerPos == PointPos::start)
					{
						temp2.type = 9;
					}
					temp.push_back(temp2);
				}
			}			
		}
		inersection_point.push_back(temp);
	}

	

	//单元格计算
	std::vector<TrimCell> ResultVector;

	std::vector<std::vector<TablePoint>> inersection_point_copy;
	inersection_point_copy = inersection_point;

	for(int i = 0 ; i < inersection_point_copy.size() ; i++)
	{
		for(int j = 0 ; j < inersection_point_copy[i].size();j++)
		{
			//if(inersection_point_copy[i][j].Searched)
			//{
			//	continue;
			//}
			if(inersection_point_copy[i][j].type == 7 || inersection_point_copy[i][j].type == 8
				||inersection_point_copy[i][j].type == 4 || inersection_point_copy[i][j].type == 5)
			{
				TrimCell tempCell = FindTrimCell(inersection_point_copy,i,j,MapHorLineNum,MapVerLineNum);
				if(tempCell.RightBottomColumn != -1)
				{
					ResultVector.push_back(tempCell);
				}
			}
		}
	}

	//尽量缩小单元格大小，并可以包含文字




	TrimCell endCell;
	endCell.LeftTopColumn = -1;
	endCell.LeftTopRow = -1;
	endCell.RightBottomColumn = -1;
	endCell.RightBottomRow = -1;

	ResultVector.push_back(endCell);

	for(int i = 0;i < ResultVector.size();i++)
	{
		CellResult[i] = ResultVector[i];
	}
}


Cell FindCell(std::vector<std::vector<TablePoint>> inersection_point,int i,int j)
{
	Cell result;
	result.LeftTopColumn = inersection_point[i][j].Column;
	result.LeftTopRow = inersection_point[i][j].Row;
	result.RightBottomColumn = -1;

	//TablePoint RightTop;
	
	int n;
	bool findedRightTop = false;
	for(n = j + 1; n <inersection_point[i].size();n++)
	{
		if(inersection_point[i][n].type == 8 || inersection_point[i][n].type == 9
			||inersection_point[i][n].type == 5 || inersection_point[i][n].type == 6)
		{
			findedRightTop = true;
			//RightTop = inersection_point[i][n];
			break;
		}
	}
	
	bool findedRightBottom = false;
	for(int m = i + 1; m <inersection_point.size();m++)
	{
		for(int k = 0; k < inersection_point[m].size();k++)
		{
			if(inersection_point[m][k].Column != inersection_point[i][n].Column)
				continue;

			if(inersection_point[m][k].type == 2 || inersection_point[m][k].type == 3
				||inersection_point[m][k].type == 5 || inersection_point[m][k].type == 6)
			{
				//RightTop = inersection_point[i][n];
				result.RightBottomColumn = inersection_point[m][k].Column;
				result.RightBottomRow = inersection_point[m][k].Row;
				findedRightBottom = true;
				break;
			}
		}
		if(findedRightBottom)
				break;
	}
	
	return result;
}


TrimCell FindTrimCell(std::vector<std::vector<TablePoint>> inersection_point,int i,int j,std::map<int,int> MapHorLineNum,std::map<int,int> MapVerLineNum)
{
	TrimCell result;
	result.LeftTopColumn = inersection_point[i][j].Column;
	result.LeftTopRow = inersection_point[i][j].Row;
	result.RightBottomColumn = -1;

	//TablePoint RightTop;
	
	int n;
	bool findedRightTop = false;
	for(n = j + 1; n <inersection_point[i].size();n++)
	{
		if(inersection_point[i][n].type == 8 || inersection_point[i][n].type == 9
			||inersection_point[i][n].type == 5 || inersection_point[i][n].type == 6)
		{
			//RightTop = inersection_point[i][n];
			findedRightTop = true;
			break;
		}
	}


	bool findedRightBottom = false;
	if(findedRightTop)
	{
		for(int m = i + 1; m <inersection_point.size();m++)
		{
			for(int k = 0; k < inersection_point[m].size();k++)
			{
				if(inersection_point[m][k].Column != inersection_point[i][n].Column)
					continue;
				if(inersection_point[m][k].type == 2 || inersection_point[m][k].type == 3
					||inersection_point[m][k].type == 5 || inersection_point[m][k].type == 6)
				{
					//RightTop = inersection_point[i][n];
					result.RightBottomColumn = inersection_point[m][k].Column;
					result.RightBottomRow = inersection_point[m][k].Row;

					findedRightBottom = true;
					break;
				}
			}

			if(findedRightBottom)
				break;
		}
	}

	if(findedRightBottom)
	{
		result.StartRow = MapHorLineNum[result.LeftTopRow];
		result.EndRow = MapHorLineNum[result.RightBottomRow] - 1;

		result.StartColumn = MapVerLineNum[result.LeftTopColumn];
		result.EndColumn = MapVerLineNum[result.RightBottomColumn] - 1;
	}	
	
	return result;
}







