package com.ajadhav4.cloud.terasort_hadoop;

import java.io.IOException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class TeraSortApplication {

	public static void main(String[] args) throws IOException, ClassNotFoundException, InterruptedException {
		long start = System.currentTimeMillis();
		Configuration conf = new Configuration();
		Job job = Job.getInstance(conf, "TeraSortHadoopJob");
		job.setJarByClass(TeraSortApplication.class); 
		job.setMapperClass(TeraSortMapper.class); 
		job.setReducerClass(TeraSortReducer.class); 
		job.setOutputKeyClass(Text.class); 
		job.setOutputValueClass(Text.class); 
		FileInputFormat.addInputPath(job, new Path(args[0])); 
		FileOutputFormat.setOutputPath(job, new Path(args[1])); 
		job.waitForCompletion(true); 
		long totalTime = System.currentTimeMillis() - start;
		System.out.println("Execution time : " + totalTime);
	}

}
