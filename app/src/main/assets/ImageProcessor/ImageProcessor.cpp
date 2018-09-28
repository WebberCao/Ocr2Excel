// ImageProcessor.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "ImageProcessor.h"


// 这是导出变量的一个示例
IMAGEPROCESSOR_API int nImageProcessor=0;

// 这是导出函数的一个示例。
extern "C"
{

IMAGEPROCESSOR_API double fnImageTableRecognise(int *Image,int row,int column,TrimCell *CellResult)
{
	
	//图像二值化
	ImageBinaryzation(Image,row,column);
	//图像倾斜校正
	double TiltAngle = TiltCorrection(Image,row,column);

	int *Lineimage = new int[row * column];
	//memset(Lineimage, 0, sizeof(int) * row * column);
	//表格线提取
	TableLineDetect(Image,Lineimage,row,column);	

	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < column; j++)
		{
			//Image[i * column + j] =  Lineimage[i * column + j];
			Image[i * column + j] = 255 - abs(Image[i * column + j] - Lineimage[i * column + j]);
		}
	}

	//表格线校正
	ThiningTableLine(Lineimage,row,column);	
	
	std::vector<HorLine> AllHorLine;
	std::vector<VerLine> AllVerLine;
	TableLineCorrection(Lineimage,row,column,15,15,25,25,AllHorLine,AllVerLine);
	TableLineCorrection(Lineimage,row,column,row / 50,column / 50,row / 50,column / 50,AllHorLine,AllVerLine);
	
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
	
	//单元格提取
	CellExact(AllHorLine, AllVerLine,CellResult);	

	//返回图像旋转角度
	return TiltAngle;
}

}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 ImageProcessor.h
CImageProcessor::CImageProcessor()
{
	return;
}
