���룺
	# javac -classpath /home/dss/lib/mariadb-java-client-1.1.5.jar:/home/dss/lib/ojdbc-11.2.0.3.jar tools.java
	����tools.class��PendingTran.class���������/home/dss/�С��������ڵ�Ŀ¼��Ҫ��dss.conf�����ļ�(�м�����������)�����԰�class�ļ���/home/dss/�С�

���У�
	# java -classpath /home/dss/lib/mariadb-java-client-1.1.5.jar:/home/dss/lib/ojdbc-11.2.0.3.jar:. tools

	�����ýű���
	# cat tools.sh 
	java -classpath /home/dss/lib/activemq-all-5.14.2.jar:/home/dss/lib/mariadb-java-client-1.1.5.jar:/home/dss/lib/ojdbc-11.2.0.3.jar:. tools $1 $2
	# ./tools.sh 