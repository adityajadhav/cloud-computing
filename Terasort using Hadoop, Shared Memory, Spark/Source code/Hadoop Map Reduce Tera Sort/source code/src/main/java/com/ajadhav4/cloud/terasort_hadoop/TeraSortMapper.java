package com.ajadhav4.cloud.terasort_hadoop;

import java.io.IOException;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

public class TeraSortMapper extends Mapper<Object, Text, Text, Text> {

	Text keyText = new Text();
	Text valueText = new Text();

	public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
		String line = value.toString();
		String k = line.substring(0, 10);
		String v = line.substring(10);
		keyText.set(k);
		valueText.set(v);
		context.write(keyText, valueText);
	}

}
