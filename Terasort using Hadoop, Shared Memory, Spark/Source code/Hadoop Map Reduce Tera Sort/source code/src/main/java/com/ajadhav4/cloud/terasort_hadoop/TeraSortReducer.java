package com.ajadhav4.cloud.terasort_hadoop;

import java.io.IOException;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

public class TeraSortReducer extends Reducer<Object, Text, Text, Text> {
	
	Text keyText = new Text();
	Text valText = new Text();

	public void reduce(Text key, Text value, Context context) throws IOException, InterruptedException {
		keyText.set(key.toString() + value.toString());
		valText.set("");
		context.write(keyText, valText);
	}
}
