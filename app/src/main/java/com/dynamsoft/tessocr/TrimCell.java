package com.dynamsoft.tessocr;

public class TrimCell {
	private int LeftTopRow;	//单元格左上角的纵坐标像素位置
	private int LeftTopColumn;	//单元格左上角的横坐标像素位置
	private int RightBottomRow;	//单元格右下角的纵坐标像素位置
	private int RightBottomColumn;	//单元格右下角的横坐标像素位置

	private int StartRow;	 //在原表格的开始行
	private int EndRow;	 //在原表格的结束行
	private int StartColumn;	//在原表格的开始列
	private int EndColumn;		//在表格的结束列
	public int getLeftTopRow() {
		return LeftTopRow;
	}
	public void setLeftTopRow(int leftTopRow) {
		LeftTopRow = leftTopRow;
	}
	public int getLeftTopColumn() {
		return LeftTopColumn;
	}
	public void setLeftTopColumn(int leftTopColumn) {
		LeftTopColumn = leftTopColumn;
	}
	public int getRightBottomRow() {
		return RightBottomRow;
	}
	public void setRightBottomRow(int rightBottomRow) {
		RightBottomRow = rightBottomRow;
	}
	public int getRightBottomColumn() {
		return RightBottomColumn;
	}
	public void setRightBottomColumn(int rightBottomColumn) {
		RightBottomColumn = rightBottomColumn;
	}
	public int getStartRow() {
		return StartRow;
	}
	public void setStartRow(int startRow) {
		StartRow = startRow;
	}
	public int getEndRow() {
		return EndRow;
	}
	public void setEndRow(int endRow) {
		EndRow = endRow;
	}
	public int getStartColumn() {
		return StartColumn;
	}
	public void setStartColumn(int startColumn) {
		StartColumn = startColumn;
	}
	public int getEndColumn() {
		return EndColumn;
	}
	public void setEndColumn(int endColumn) {
		EndColumn = endColumn;
	}
	@Override
	public String toString() {
		return "TrimCell [LeftTopRow=" + LeftTopRow + ", LeftTopColumn="
				+ LeftTopColumn + ", RightBottomRow=" + RightBottomRow
				+ ", RightBottomColumn=" + RightBottomColumn + ", StartRow="
				+ StartRow + ", EndRow=" + EndRow + ", StartColumn="
				+ StartColumn + ", EndColumn=" + EndColumn + "]";
	}
}
