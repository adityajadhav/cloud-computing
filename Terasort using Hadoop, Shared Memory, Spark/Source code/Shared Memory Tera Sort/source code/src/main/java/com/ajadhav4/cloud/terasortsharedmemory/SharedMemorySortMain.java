package com.ajadhav4.cloud.terasortsharedmemory;

import java.io.File;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class SharedMemorySortMain {

	private static Reader reader;

	public static void main(String[] args) {

		reader = Reader.getInstance();

		System.out.println("Shared Memory Sort");
		String test = reader.sourceFolder + File.separator + reader.fileName;
		System.out.println("Source file " + test);
		System.out.println("Destination folder " + reader.destFolder);
		System.out.println("Number of Threads in Thread Pool " + reader.numThreads);
		// displayConfig();

		long start = System.currentTimeMillis();
		mapper();
		reducer();
		long executionTime = System.currentTimeMillis() - start;

		System.out.println("Execution time : " + executionTime);

	}

	private static void mapper() {

		long start = System.currentTimeMillis();

		reader = Reader.getInstance();
		String sourceFilePath = reader.sourceFolder + File.separator + reader.fileName;
		File file = new File(sourceFilePath);

		if (file != null && file.length() > 1) {
			long numSplits = getNumSplits(file.length());

			ExecutorService executor = Executors.newFixedThreadPool(reader.numThreads);

			for (int i = 0; i < numSplits; i++) {
				Runnable spliter = new SpliterTask(i);
				executor.execute(spliter);
			}

			executor.shutdown();
			while (!executor.isTerminated()) {
			}
		} else {
			System.out.println("Source file not found");
		}

		long executionTime = System.currentTimeMillis() - start;

		System.out.println("File Splitted, Execution time " + executionTime);

	}

	private static void reducer() {

		File folder = new File(reader.destFolder);
		if (folder != null) {
			File[] files = folder.listFiles();
			int itr = 0;
			if (files != null) {
				while (files.length > 1) {
					int mergeItr = 0;
					int threadId = 0;
					ExecutorService executor = Executors.newFixedThreadPool(reader.numThreads);
					while ((mergeItr + 1) < files.length) {
						Runnable mergeTask = new MergeTask(threadId, itr, files[mergeItr], files[mergeItr + 1]);
						executor.execute(mergeTask);
						threadId++;
						mergeItr = mergeItr + 2;
					}
					executor.shutdown();
					while (!executor.isTerminated()) {
					}
					itr++;
					folder = new File(reader.destFolder);
					files = folder.listFiles();
				}
			}
		}
	}

	private static long getNumSplits(long numBytes) {
		return (numBytes / reader.bytesPerLine) / reader.linesPerSplit;
	}
}
