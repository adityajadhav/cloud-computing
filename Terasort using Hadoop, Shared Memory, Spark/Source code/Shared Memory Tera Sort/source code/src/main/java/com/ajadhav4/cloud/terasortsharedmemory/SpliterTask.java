package com.ajadhav4.cloud.terasortsharedmemory;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.Arrays;
import java.util.TreeMap;

public class SpliterTask implements Runnable {

	private int threadId = 0;
	private Reader reader;

	public SpliterTask(int threadId) {
		this.setThreadNum(threadId);
		this.reader = Reader.getInstance();
	}

	@Override
	public void run() {

		String threadName = Thread.currentThread().getName();
		// System.out.println(threadName);
		String holder;
		int cnt = 0;
		TreeMap<String, String> map = new TreeMap<String, String>();
		int size = (int) reader.linesPerSplit * reader.bytesPerLine;
		byte[] bytesPerLine = new byte[reader.bytesPerLine];
		byte[] data = new byte[size];
		FileOutputStream fileOutputStream = null;
		RandomAccessFile randomAccessFile = null;

		try {

			fileOutputStream = new FileOutputStream(
					reader.destFolder + File.separatorChar + "split_" + this.getThreadNum());
			randomAccessFile = new RandomAccessFile(reader.sourceFolder + File.separatorChar + reader.fileName, "r");

			randomAccessFile.seek(this.getThreadNum() * reader.bytesPerLine * reader.linesPerSplit);
			randomAccessFile.read(data, 0, size);

			for (int a = 0; a < reader.linesPerSplit; a++) {
				int start = a * reader.bytesPerLine;
				bytesPerLine = Arrays.copyOfRange(data, start, start + reader.bytesPerLine);
				holder = new String(bytesPerLine);
				map.put(holder.substring(0, 10), holder.substring(10));
			}

			
			for (String key : map.keySet()) {
				holder = key + map.get(key);
				int start = cnt * 100;
				bytesPerLine = holder.getBytes();
				for (int b = 0; b < bytesPerLine.length; b++) {
					data[start] = bytesPerLine[b];
					start++;
				}
				cnt++;
			}

			fileOutputStream.write(data, 0, size);

		} catch (IOException e) {
			e.printStackTrace();
		} finally {

			try {
				fileOutputStream.flush();
				fileOutputStream.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}

	}

	public int getThreadNum() {
		return threadId;
	}

	public void setThreadNum(int threadNum) {
		this.threadId = threadNum;
	}

}
