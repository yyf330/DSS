编译：
	# javac -classpath /home/dss/lib/mariadb-java-client-1.1.5.jar:/home/dss/lib/ojdbc-11.2.0.3.jar tools.java
	生成tools.class、PendingTran.class，将其放入/home/dss/中。程序所在的目录需要有dss.conf配置文件(中间库的属性配置)，所以把class文件放/home/dss/中。

运行：
	# java -classpath /home/dss/lib/mariadb-java-client-1.1.5.jar:/home/dss/lib/ojdbc-11.2.0.3.jar:. tools

	或者用脚本：
	# cat tools.sh 
	java -classpath /home/dss/lib/activemq-all-5.14.2.jar:/home/dss/lib/mariadb-java-client-1.1.5.jar:/home/dss/lib/ojdbc-11.2.0.3.jar:. tools $1 $2
	# ./tools.sh 