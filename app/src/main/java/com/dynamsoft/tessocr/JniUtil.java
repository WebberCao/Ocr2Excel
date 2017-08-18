package com.dynamsoft.tessocr;

import java.util.ArrayList;

import android.graphics.Bitmap;

public class JniUtil {
	static {
        System.loadLibrary("imageprocessor");
    }
	public static native ArrayList<TrimCell> imageRecognise(Bitmap bitmap);
}
