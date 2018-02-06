import java.io.{File, BufferedWriter, FileWriter}

val start = System.currentTimeMillis()
val input = "hdfs:///ajadhav4/input/128gb"
val file = new File("ouput")
val bw = new BufferedWriter(new FileWriter(file))
val data = sc.textFile(input)
val splitData = data.flatMap(line => line.split("\n"))
val arrayFromCollectedData = splitData.flatMap(line => Map(line.substring(0,10) -> line.substring(10,line.length)))
val mapFromArray = arrayFromCollectedData.map(line => line._1 -> line._2)
val sortedData = mapFromArray.sortBy(_._1)
sortedData.collect().foreach(line => bw.write(line._1 + line._2 + "\r\n"))
val executionTime = System.currentTimeMillis() - start
println ("Execution time (ms): " + executionTime)
