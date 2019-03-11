package org.apache.hadoop.concu;

import java.nio.ByteBuffer;
import java.util.StringTokenizer;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.WritableComparator;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;

public class Sort {
  public static void main(String[] args) throws Exception {
    Path inputPath = new Path(args[0]);
    Path outputDir = new Path(args[1]);

    Configuration conf = new Configuration(true);

    Job job = Job.getInstance(conf, "wordcount sort");
    job.setJarByClass(Sort.class);

    job.setMapperClass(Sort.MapTask.class);
    job.setReducerClass(Sort.ReduceTask.class);
    job.setNumReduceTasks(1);

    job.setMapOutputKeyClass(IntWritable.class);
    job.setMapOutputValueClass(Text.class);
    job.setOutputKeyClass(IntWritable.class);
    job.setOutputValueClass(Text.class);
    job.setSortComparatorClass(IntComparator.class);

    FileInputFormat.addInputPath(job, inputPath);
    job.setInputFormatClass(TextInputFormat.class);

    FileOutputFormat.setOutputPath(job, outputDir);
    job.setOutputFormatClass(TextOutputFormat.class);

    System.exit(job.waitForCompletion(true) ? 0 : 1);
  }

  public static class IntComparator extends WritableComparator {
    public IntComparator() {
      super(IntWritable.class);
    }

    @Override
    public int compare(byte[] b1, int s1, int l1,
                       byte[] b2, int s2, int l2) {
      Integer v1 = ByteBuffer.wrap(b1, s1, l1).getInt();
      Integer v2 = ByteBuffer.wrap(b2, s2, l2).getInt();
      return v1.compareTo(v2) * (-1);
    }
  }

  public static class MapTask extends Mapper<LongWritable, Text, IntWritable, Text> {
    public void map(LongWritable key, Text value, Context context) throws java.io.IOException, InterruptedException {
      StringTokenizer itr = new StringTokenizer(value.toString());

      String first = itr.nextToken();
      String second = itr.nextToken();
      IntWritable count = new IntWritable(Integer.parseInt(itr.nextToken()));

      context.write(count, new Text(first + ' ' + second));
    }
  }

  public static class ReduceTask extends Reducer<IntWritable, Text, Text, IntWritable> {
    public void reduce(IntWritable key, Iterable<Text> list, Context context) throws java.io.IOException, InterruptedException {
      for (Text value : list) {
        context.write(value,key);
      }
    }
  }
}
