# Ocr2Excel
一个基于OCR识别引擎的识别表格文字并将结果以Excel电子表格的形式原样导出的Android客户端工具

2018/09/17更新：修复了识别完成之后找不到保存的Excel文件的问题。
## 界面截图
![image]( https://github.com/WebberCao/Ocr2Excel/raw/master/app/src/main/assets/image11.jpg )
![image]( https://github.com/WebberCao/Ocr2Excel/raw/master/app/src/main/assets/image22.jpg )
![image]( https://github.com/WebberCao/Ocr2Excel/raw/master/app/src/main/assets/image33.jpg )
![image]( https://github.com/WebberCao/Ocr2Excel/raw/master/app/src/main/assets/image44.jpg )
## 实现思路
1. 对表格图片进行灰度化和二值化处理
2. 对图像进行倾斜矫正
3. 进行表格线提取
4. 进行表格线矫正
5. 单元格提取
6. 根据返回的每个单元格的像素位置对原图像进行剪裁
6. 对剪裁得到的内容逐个进行Ocr识别
7. 将识别的内容根据返回的位置数据写入Excel表格

其中的1~5步考虑到效率问题，图像算法研究的同事帮忙用C++代码编写，自己做了转换处理和封装，编译成了so文件在代码中进行调用。
## 相关代码说明
```javascript
public class TrimCell {
	private int LeftTopRow;	  //单元格左上角的纵坐标像素位置
	private int LeftTopColumn;	//单元格左上角的横坐标像素位置
	private int RightBottomRow;  //单元格右下角的纵坐标像素位置
	private int RightBottomColumn;	//单元格右下角的横坐标像素位置

	private int StartRow;	//在原表格的开始行
	private int EndRow;	    //在原表格的结束行
	private int StartColumn;  //在原表格的开始列
	private int EndColumn;	   //在表格的结束列
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
```
根据TrimCell中的第一组位置数据，对原图像进行裁剪：
```javascript
Bitmap bitmaptemp = Bitmap.createBitmap(bitmap, trimCell.getLeftTopColumn(),
trimCell.getLeftTopRow(),Math.abs(trimCell.getRightBottomColumn()-trimCell.getLeftTopColumn()), trimCell.getRightBottomRow()-trimCell.getLeftTopRow());
```
根据TrimCell中的第二组位置数据，使用jxl将识别出的内容写入Excel表格中：
```javascript
Label label = new Label(trimcelllist.get(i).getStartColumn(),
trimcelllist.get(i).getStartRow(), datas.get(i));
ws.addCell(label);
```
判断是否需要合并单元格：
```javascript
if(trimcelllist.get(i).getStartRow()!=trimcelllist.get(i).getEndRow() || trimcelllist.get(i).getStartColumn()!=trimcelllist.get(i).getEndColumn())
{
   ws.mergeCells(trimcelllist.get(i).getStartColumn(),trimcelllist.get(i).getStartRow(), trimcelllist.get(i).getEndColumn(),trimcelllist.get(i).getEndRow());
}
```

#### 如果有什么错误或者建议欢迎随时指出，大家可以讨论一起进步。如果您觉得对您有用，请给个Star，谢谢~
