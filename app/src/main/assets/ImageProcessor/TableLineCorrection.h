
#include <vector>

struct HorLine
{
	int StartColumn;
	int EndColumn;
	int NthRow;
};

struct VerLine
{
	int StartRow;
	int EndRow;
	int NthColumn;
};


struct DoubleHorLine
{
	int StartColumn;
	int EndColumn;
	double NthRow;
};

struct DoubleVerLine
{
	int StartRow;
	int EndRow;
	double NthColumn;
};

void TableLineCorrection(int *image,int row,int column,int RowLineDistance,int ColumnLineDistance,int CombinedDeltaRowLine,int CombinedDeltaColumnLine,std::vector<HorLine> &AllHorLine,std::vector<VerLine> &AllVerLine);
void ThiningTableLine(int *image,int row,int column);
std::vector<HorLine> HorLineExtract(int *image,int row,int column,int CombinedDeltaLine);
std::vector<VerLine> VerLineExtract(int *image,int row,int column,int CombinedDeltaLine);


std::vector<HorLine> PreciseHorLineExtract(int *image,int row,int column,int CombinedDeltaLine);
std::vector<VerLine> PreciseVerLineExtract(int *image,int row,int column,int CombinedDeltaLine);