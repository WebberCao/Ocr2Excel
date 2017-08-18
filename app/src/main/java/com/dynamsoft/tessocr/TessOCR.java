package com.dynamsoft.tessocr;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import android.content.Context;
import android.graphics.Bitmap;
import android.os.Environment;
import android.widget.Toast;

import com.googlecode.tesseract.android.TessBaseAPI;
import com.dynamsoft.orc2excel.R;

public class TessOCR {
	
	private TessBaseAPI mTess;
	private Context mContext;
	private String TESSBASE_PATH;
    private String TESSBASE_DATA;
            //Environment.getExternalStorageDirectory().getAbsolutePath()+"/tesseract/tessdata";
    private static final String CHINESE_LANGUAGE = "chi_sim";
	
	public TessOCR(Context context) {
		mContext = context;
		mTess = new TessBaseAPI();
        TESSBASE_PATH = mContext.getFilesDir().getAbsolutePath();
        TESSBASE_DATA = TESSBASE_PATH + "/tessdata";
		initData();
		mTess.init(TESSBASE_PATH, CHINESE_LANGUAGE);
	}
	
	public String getOCRResult(Bitmap bitmap) {
		
		mTess.setImage(bitmap);
		String result = mTess.getUTF8Text();

		return result;
    }
	
	public void onDestroy() {
		if (mTess != null)
			mTess.end();
	}
	
	public void initData(){
        try {
            String status = Environment.getExternalStorageState();
            if (!status.equals(Environment.MEDIA_MOUNTED)) {
                Toast.makeText(mContext,"检测不到sd卡，请确认sd卡已经插入",Toast.LENGTH_SHORT).show();
                return;
            }
            File folder = new File(TESSBASE_DATA);
            if (folder.exists() && folder.isDirectory()) {
                //do nothing
            } else {
                folder.mkdirs();
            }
            copyRawToSdcard(mContext,folder);

        }catch (Exception e){
            e.printStackTrace();
        }
	}
	
    private  void copyRawToSdcard(Context context, File folder){
        File file=new File(folder,"chi_sim.traineddata");
        BufferedInputStream bufferedInputStream=null;
        BufferedOutputStream bufferedOutputStream=null;
        try {
            file.createNewFile();
            bufferedInputStream = new BufferedInputStream(context.getResources().openRawResource(R.raw.chi_sim));
            bufferedOutputStream = new BufferedOutputStream(new FileOutputStream(file));
            byte[] buff=new byte[20*1024];
            int len;
            while ((len= bufferedInputStream.read(buff))>0){
                bufferedOutputStream.write(buff,0,len);
            }
            bufferedOutputStream.flush();
            bufferedOutputStream.close();
            bufferedInputStream.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }
	
}
