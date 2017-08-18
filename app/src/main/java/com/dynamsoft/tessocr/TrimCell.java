package com.dynamsoft.tessocr;

public class TrimCell {
	private int LeftTopRow;
	private int LeftTopColumn;
	private int RightBottomRow;
	private int RightBottomColumn;

	private int StartRow; 
	private int EndRow;
	private int StartColumn;
	private int EndColumn;
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
