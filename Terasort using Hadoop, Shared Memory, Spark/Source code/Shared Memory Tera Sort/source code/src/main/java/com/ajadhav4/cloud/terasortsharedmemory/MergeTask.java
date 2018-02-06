package com.ajadhav4.cloud.terasortsharedmemory;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

public class MergeTask implements Runnable {

	private Reader reader = null;
	private File f1, f2;
	private int threadId, itr;

	public MergeTask(int threadId, int itr, File f1, File f2) {
		reader = Reader.getInstance();
		this.threadId = threadId;
		this.f1 = f1;
		this.f2 = f2;
		this.itr = itr;
	}

	@Override
	public void run() {

		BufferedReader br1 = null, br2 = null;

		try {

			br1 = new BufferedReader(new FileReader(f1));
			br2 = new BufferedReader(new FileReader(f2));
			BufferedWriter bufWritter = new BufferedWriter(
					new FileWriter("destination" + File.separator + "merge_" + itr + "_" + threadId));
			String key1, key2;
			String line1 = br1.readLine(), line2 = br2.readLine();
			while (null != line1 && null != line2) {
				key1 = line1.substring(0, 10);
				key2 = line2.substring(0, 10);
				if (key1.compareTo(key2) <= 0) {
					bufWritter.write(line1 + "\r\n");
					line1 = br1.readLine();
				} else {
					bufWritter.write(line2 + "\r\n");
					line2 = br2.readLine();
				}
			}

			while (line1 != null) {
				bufWritter.write(line1 + "\r\n");
				line1 = br1.readLine();
			}

			while (line2 != null) {
				bufWritter.write(line2 + "\r\n");
				line2 = br2.readLine();
			}
			bufWritter.flush();
			bufWritter.close();

		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			try {
				br1.close();
				br2.close();
				Files.delete(Paths.get(f1.getPath()));
				Files.delete(Paths.get(f2.getPath()));
			} catch (IOException e) {
				e.printStackTrace();
			}

		}
	}

}
