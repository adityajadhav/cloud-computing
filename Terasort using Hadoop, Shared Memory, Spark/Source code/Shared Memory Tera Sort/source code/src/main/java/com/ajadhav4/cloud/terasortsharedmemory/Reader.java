package com.ajadhav4.cloud.terasortsharedmemory;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;

public class Reader {

	private static Reader reader = null;
	public String sourceFolder;
	public String fileName;
	public String destFolder;
	public int numThreads;
	public int bytesPerLine;
	public long linesPerSplit;

	private Reader() {

		String filePath = "config.properties";
		Properties prop = new Properties();

		//try (InputStream inputStream = Thread.currentThread().getContextClassLoader().getResourceAsStream("config.properties")){
		try (InputStream inputStream = new FileInputStream(new File("./" + filePath))) {

			prop.load(inputStream);
			this.sourceFolder = prop.getProperty("source.folder");
			this.fileName = prop.getProperty("source.file");
			this.destFolder = prop.getProperty("destination.folder");
			this.numThreads = Integer.parseInt(prop.getProperty("threads"));
			this.linesPerSplit = Long.parseLong(prop.getProperty("lines.per.split"));
			this.bytesPerLine = Integer.parseInt(prop.getProperty("source.file.bytes.per.line"));

		} catch (IOException ex) {
			System.out.println("Error reading file");
			ex.printStackTrace();
		}

	}

	public static Reader getInstance() {
		if (reader == null) {
			reader = new Reader();
		}
		return reader;
	}

}
