package com.dynamsoft.tessocr;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import jxl.Workbook;
import jxl.write.Label;
import jxl.write.WritableSheet;
import jxl.write.WritableWorkbook;
import android.app.Activity;
import android.app.ProgressDialog;
import android.app.AlertDialog.Builder;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.StatFs;
import android.os.StrictMode;
import android.provider.MediaStore;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Toast;

import com.dynamsoft.orc2excel.R;

public class OCRActivity extends Activity implements OnClickListener {
	private TessOCR mTessOCR;
	private ProgressDialog mProgressDialog;
	private ImageView mImage;
	private Button mButtonGallery, mButtonCamera;
	private String mCurrentPhotoPath;
	private static final int REQUEST_TAKE_PHOTO = 1;
	private static final int REQUEST_PICK_PHOTO = 2;
	private File file;
	
	private List<String> datas = new ArrayList<String>();
	private ArrayList<TrimCell> trimcelllist;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		mImage = (ImageView) findViewById(R.id.image);
		mButtonGallery = (Button) findViewById(R.id.bt_gallery);
		mButtonGallery.setOnClickListener(this);
		mButtonCamera = (Button) findViewById(R.id.bt_camera);
		mButtonCamera.setOnClickListener(this);
		mTessOCR = new TessOCR(getApplicationContext());
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
			StrictMode.VmPolicy.Builder builder = new StrictMode.VmPolicy.Builder();
			StrictMode.setVmPolicy(builder.build());
		}
	}

	private void uriOCR(Uri uri) {
		if (uri != null) {
			InputStream is = null;
			Bitmap bitmap_ocr = null;
			try {
				is = getContentResolver().openInputStream(uri);
			int targetW = mImage.getWidth();
			int targetH = mImage.getHeight();
			// Get the dimensions of the bitmap
			BitmapFactory.Options bmOptions = new BitmapFactory.Options();
			bmOptions.inJustDecodeBounds = true;
			Bitmap bitmap_temp = BitmapFactory.decodeStream(is, null, bmOptions);
			int photoW = bmOptions.outWidth;
			int photoH = bmOptions.outHeight;
			// Determine how much to scale down the image
			int scaleFactor = Math.min(photoW / targetW, photoH / targetH);
			// Decode the image file into a Bitmap sized to fill the View
			bmOptions.inJustDecodeBounds = false;
			bmOptions.inSampleSize = scaleFactor << 1;
			Bitmap bitmap = BitmapFactory.decodeStream(getContentResolver().openInputStream(uri), null, bmOptions);
			bitmap_ocr = BitmapFactory.decodeStream(getContentResolver().openInputStream(uri));
			mImage.setImageBitmap(bitmap);
			}catch (Exception e) {
				e.printStackTrace();
			}
			doOCR(uri);
		}
	}
	
	@Override
	protected void onResume() {
		super.onResume();

	}

	@Override
	protected void onPause() {
		super.onPause();
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		mTessOCR.onDestroy();
	}
	
	//保存图片
	private void savePhoto(Bitmap bitmap,int i){
		File file;
		File dir = Environment.getExternalStorageDirectory();
		file = new File(dir, "temp_"+i+".jpg");
		try {
			file.createNewFile();
			FileOutputStream fos = new FileOutputStream(file);
			bitmap.compress(CompressFormat.JPEG, 80, fos);
			fos.flush();
			fos.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
	}

	private void dispatchTakePictureIntent() {
		Intent takePictureIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
		// Ensure that there's a camera activity to handle the intent
		if (takePictureIntent.resolveActivity(getPackageManager()) != null) {
			// Create the File where the photo should go
			File photoFile = null;
			try {
				photoFile = createImageFile();
			} catch (IOException ex) {
				// Error occurred while creating the File

			}
			// Continue only if the File was successfully created
			if (photoFile != null) {
				takePictureIntent.putExtra(MediaStore.EXTRA_OUTPUT,
						Uri.fromFile(photoFile));
				startActivityForResult(takePictureIntent, REQUEST_TAKE_PHOTO);
			}
		}
	}

	private File createImageFile() throws IOException {
		File image = null;
		try{
			// Create an image file name
			String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss")
					.format(new Date());
			String imageFileName = "JPEG_" + timeStamp + "_";
			String storageDir = getFilesDir().getAbsolutePath()
					+ "/TessOCR";
			File dir = new File(storageDir);
			if (!dir.exists())
				dir.mkdir();
			image = new File(dir, imageFileName + ".jpg");
			image.createNewFile();
			mCurrentPhotoPath = image.getAbsolutePath();
		}catch (Exception e){

		}
		return image;
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (requestCode == REQUEST_TAKE_PHOTO
				&& resultCode == Activity.RESULT_OK) {
			setPic();
		}
		else if (requestCode == REQUEST_PICK_PHOTO
				&& resultCode == Activity.RESULT_OK) {
			Uri uri = data.getData();
			if (uri != null) {
				uriOCR(uri);
			}
		}
	}

	private void setPic() {
		// Get the dimensions of the View
		int targetW = mImage.getWidth();
		int targetH = mImage.getHeight();
		// Get the dimensions of the bitmap
		BitmapFactory.Options bmOptions = new BitmapFactory.Options();
		bmOptions.inJustDecodeBounds = true;
		BitmapFactory.decodeFile(mCurrentPhotoPath, bmOptions);
		int photoW = bmOptions.outWidth;
		int photoH = bmOptions.outHeight;
		// Determine how much to scale down the image
		int scaleFactor = Math.min(photoW / targetW, photoH / targetH);
		// Decode the image file into a Bitmap sized to fill the View
		bmOptions.inJustDecodeBounds = false;
		bmOptions.inSampleSize = scaleFactor << 1;
		bmOptions.inPurgeable = true;
		Bitmap bitmap = BitmapFactory.decodeFile(mCurrentPhotoPath, bmOptions);
		mImage.setImageBitmap(bitmap);
		doOCR();
	}

	@Override
	public void onClick(View v) {
		int id = v.getId();
		switch (id) {
		case R.id.bt_gallery:
			pickPhoto();
			break;
		case R.id.bt_camera:
			takePhoto();
			break;
		}
	}
	
	private void pickPhoto() {
		Intent intent = new Intent(Intent.ACTION_PICK, MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
		startActivityForResult(intent, REQUEST_PICK_PHOTO);
	}

	private void takePhoto() {
		dispatchTakePictureIntent();
	}
	
	private Bitmap pieceBitmap(Uri uri){
		Bitmap bitmap_ocr =null;
		try {
			bitmap_ocr = BitmapFactory.decodeStream(getContentResolver().openInputStream(uri));
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		trimcelllist =  JniUtil.imageRecognise(bitmap_ocr);
		return bitmap_ocr;
	}
	
	private Bitmap pieceBitmap(){
		Bitmap bitmap_ocr = BitmapFactory.decodeFile(mCurrentPhotoPath);
		trimcelllist =  JniUtil.imageRecognise(bitmap_ocr);
		return bitmap_ocr;
	}
	
	private void doOCR(final Uri uri) {
		if (mProgressDialog == null) {
			mProgressDialog = ProgressDialog.show(this, "Processing",
					"正在识别...", true);
		}
		else {
			mProgressDialog.show();
		}
		new Thread(new Runnable() {
			public void run() {
				Bitmap bitmap = pieceBitmap(uri);
				for (TrimCell trimCell : trimcelllist) {
					try {
						Bitmap bitmaptemp = Bitmap.createBitmap(bitmap, trimCell.getLeftTopColumn(), trimCell.getLeftTopRow(), Math.abs(trimCell.getRightBottomColumn()-trimCell.getLeftTopColumn()), trimCell.getRightBottomRow()-trimCell.getLeftTopRow());
						//执行OCR识别
						String result = mTessOCR.getOCRResult(bitmaptemp);
						datas.add(result);
					} catch (Exception e) {
						e.printStackTrace();
					}
				}
				runOnUiThread(new Runnable() {
					@Override
					public void run() {
						//写入Excel文件中
						WriteExcel();
						Toast.makeText(getApplicationContext(), "已识别完毕！", Toast.LENGTH_LONG).show();
						mProgressDialog.dismiss();
						//打开Excel文件
						openExcel();
					}
				});
			};
		}).start();
	}

	private void doOCR() {
		if (mProgressDialog == null) {
			mProgressDialog = ProgressDialog.show(this, "Processing",
					"正在识别...", true);
		}
		else {
			mProgressDialog.show();
		}
		new Thread(new Runnable() {
			public void run() {
				Bitmap bitmap = pieceBitmap();
				for (TrimCell trimCell : trimcelllist) {
					try {
						Bitmap bitmaptemp = Bitmap.createBitmap(bitmap, trimCell.getLeftTopColumn(), trimCell.getLeftTopRow(), Math.abs(trimCell.getRightBottomColumn()-trimCell.getLeftTopColumn()), trimCell.getRightBottomRow()-trimCell.getLeftTopRow());
						//执行OCR识别
						String result = mTessOCR.getOCRResult(bitmaptemp);
						datas.add(result);
					} catch (Exception e) {
						e.printStackTrace();
					}
				}
				runOnUiThread(new Runnable() {
					@Override
					public void run() {
						//写入Excel文件中
						WriteExcel();
						Toast.makeText(getApplicationContext(), "识别完毕！", Toast.LENGTH_LONG).show();
						mProgressDialog.dismiss();
						//打开Excel文件
						openExcel();
					}

				});
				
			};
		}).start();
	}
	
	//打开Excel文件
	private void openExcel(){
		Builder builder=new Builder(OCRActivity.this);
		builder.setTitle("识别成功");
		builder.setMessage("识别结果已保存在"+file.getAbsolutePath()+"是否打开查看？");
		builder.setPositiveButton("打开", new DialogInterface.OnClickListener() {
			@Override
			public void onClick(DialogInterface dialog, int which) {
				try {
					Uri path1 = Uri.fromFile(file);
					Intent intent = new Intent(Intent.ACTION_VIEW);
					intent.setDataAndType(path1,"application/vnd.ms-excel");
					intent.addCategory("android.intent.category.DEFAULT");
					intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
					startActivity(intent);
				} catch (Exception e) {
					Toast.makeText(getApplicationContext(),
							"未检测出可以打开此类型文件的软件，请先安装Excel阅读器！", Toast.LENGTH_LONG).show();
				}
			}
		});
		builder.setNegativeButton("稍后", null);
		builder.show();
		
	}
	
	//将识别结果写入Excel文件中
	private void WriteExcel(){
		if (!Environment.getExternalStorageState().equals(
				Environment.MEDIA_MOUNTED)
				&& getAvailableStorage(getApplicationContext()) < 1) {
			Toast.makeText(getApplicationContext(), "SD卡不可用", Toast.LENGTH_LONG).show();
			return;
		}
		File dir = new File(getExternalFilesDir("apk").getAbsolutePath() + "/tessdata");
		if (!dir.exists()) {
			dir.mkdirs();
		}
		file = new File(dir, "TestWriterExcel" + ".xls");
		
		try {
			WritableWorkbook wwb = Workbook.createWorkbook(file);
			WritableSheet ws = wwb.createSheet("TestSheet1", 0);
			
			for (int i=0;i<trimcelllist.size();i++){
				//设置单元格内容
				Label label = new Label(trimcelllist.get(i).getStartColumn(), trimcelllist.get(i).getStartRow(), datas.get(i));
				ws.addCell(label);
				//判断需要合并单元格
				if(trimcelllist.get(i).getStartRow()!=trimcelllist.get(i).getEndRow() || trimcelllist.get(i).getStartColumn()!=trimcelllist.get(i).getEndColumn()){
					ws.mergeCells(trimcelllist.get(i).getStartColumn(), trimcelllist.get(i).getStartRow(), trimcelllist.get(i).getEndColumn(), trimcelllist.get(i).getEndRow());
				}
			}
			//写入文件中
			wwb.write();

			wwb.close();
			
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	 private static long getAvailableStorage(Context context) {        
		 File path = Environment.getExternalStorageDirectory();
		 StatFs statFs = new StatFs(path.getPath());        
		 long blockSize = statFs.getBlockSize();
		 long availableBlocks = statFs.getAvailableBlocks();        
		 long availableSize = blockSize * availableBlocks;        
		 return availableSize/1024/1024;    //单位MB
	 }
}
