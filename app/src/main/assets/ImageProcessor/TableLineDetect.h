
#include <queue>

struct LengthRow
{
	int Length;
	int Row;
};

struct LengthColumn
{
	int Length;
	int Column;
};

void TableLineDetect(int *image,int *Lineimage,int row,int column);
void HorLineDetect(int *image,int *Lineimage,int row,int column);

//按长度快速排序  从小到大升序排列
void LengthquickSort(LengthRow s[], int l, int r) ;

void AllLineDetect(int *image,int *Lineimage,int row,int column);
void FindStartPointEightNeibourhood(int *image,int *Lineimage,int row,int column,int i,int j);
void FindAllLineEightNeibourhood(int *image,int *Lineimage,int row,int column,int i,int j,int StartedRow);
void BFS(int *image,int *Lineimage,int row,int column, int i, int j);
void BFS_delta(int *image,int *Lineimage,int row,int column, int i, int j,int delta);
