import java.io.*;
import java.sql.*;
import java.util.*;

public class tools {
	private static Properties properties;
	private static PrintStream verboseLog;
	private static Connection mysqlConn;
	private static Connection sourceConn,destConn;//源库连接,目标库连接
	private static String mysqlHost,mysqlDbName,mysqlUser,mysqlPass,mysqlURL;//中间库连接属性
	private static String SOURCE_ID,DEST_ID,STRATEGY_ID;
	private static String SOURCE_USER,SOURCE_PWD,SOURCE_IP,SOURCE_PORT,SOURCE_INSTANCE,SOURCE_URL;
	private static String DEST_USER="SYSTEM",DEST_PWD,DEST_IP,DEST_PORT,DEST_INSTANCE,DEST_URL;

	private static void init(){
		try{
			verboseLog = new PrintStream(new FileOutputStream("tools.log", true));
		}
		catch(Exception e){
			System.out.println("handle log file error: " + e.getMessage());
			System.exit(1);
		}

		java.io.InputStream inputstream = tools.class.getResourceAsStream("/dss.conf");
		properties = new Properties();
		try{
			properties.load(inputstream);
			mysqlHost = properties.getProperty("HOST");
			mysqlUser = properties.getProperty("USER");
			mysqlPass = properties.getProperty("PASSWORD");
			mysqlDbName = properties.getProperty("DBNAME");
			mysqlURL = "jdbc:mariadb://"+mysqlHost+"/"+mysqlDbName;	//"jdbc:mariadb://127.0.0.1/DSS"
		}
		catch(Exception e){
			System.out.println("load properties file (dss.conf) error: " + e.getMessage());
			System.exit(1);
		}

		try{
			Class.forName ("org.mariadb.jdbc.Driver");
			mysqlConn = DriverManager.getConnection(mysqlURL, mysqlUser, mysqlPass);
			mysqlConn.setAutoCommit(false);
		}
		catch(Exception e){
			System.out.println("init mysql connection error: " + e.getMessage());
			System.exit(1);
		}
	}

	public static String get_string(Connection conn, String sql, String columnname){
		try{
			String s2="";
			Statement statement = conn.createStatement();
			ResultSet resultset = statement.executeQuery(sql);
			if(resultset.next()) s2 = resultset.getString(columnname).trim();
			resultset.close();
			statement.close();
			return s2;
		}
		catch(Exception e){
			System.out.println("select operation error: sql: " + sql + " : " + e.getMessage());
			return "";
		}
	}

	public static void main(String[] args) throws Exception{
		init();

		String funcType = null;
		if(args.length == 0)
			funcType = "help";
		else
			funcType = args[0];

		boolean validFlag = false;	//输入选项是否有效

		//funcType:
		//打印帮助:				java tools help
		//获取源库当前SCN:		java tools getsourcescn 源库id
		//等待源库事务结束:		java tools waittxdone 源库id

		//获得源库BLOB表名:		java tools getblobtable 源库id
		//源库创建物化视图日志:	java tools createmvlog 源库id
		//上面2步的组合:		java tools sourcemv 源库id

		//目标库建dblink:		java tools createdblink 策略id
		//目标库修改表名:		java tools renamemvtable 策略id
		//目标库创建物化视图:	java tools createmv 策略id
		//上面3步的组合:		java tools destmv 策略id

		//目标库物化视图转为表:	java mv2table 策略id

		if("help".equalsIgnoreCase(funcType)){
			//tools.sh
			//java -classpath /home/dss/lib/activemq-all-5.14.2.jar:/home/dss/lib/mariadb-java-client-1.1.5.jar:/home/dss/lib/ojdbc-11.2.0.3.jar:. tools $1 $2
			System.out.println("打印帮助:\t\t\t./tools.sh help");
			System.out.println("获取源库当前SCN:\t\t./tools.sh getsourcescn 源库id");
			System.out.println("等待源库事务结束:\t\t./tools.sh waittxdone 源库id");

			System.out.println("获得源库BLOB表名:\t\t./tools.sh getblobtable 源库id");
			System.out.println("源库创建物化视图日志:\t\t./tools.sh createmvlog 源库id");
			System.out.println("上面2步的组合:\t\t\t./tools.sh sourcemv 源库id");

			System.out.println("目标库建dblink:\t\t\t./tools.sh createdblink 策略id");
			System.out.println("目标库修改表名:\t\t\t./tools.sh renamemvtable 策略id");
			System.out.println("目标库创建物化视图:\t\t./tools.sh createmv 策略id");
			System.out.println("上面3步的组合:\t\t\t./tools.sh destmv 策略id");

			System.out.println("目标库物化视图转为表:\t\t./tools.sh mv2table 策略id");
			return;
		}

		if("createdblink".equalsIgnoreCase(funcType) || "renamemvtable".equalsIgnoreCase(funcType) || "createmv".equalsIgnoreCase(funcType)
			|| "mv2table".equalsIgnoreCase(funcType) || "destmv".equalsIgnoreCase(funcType)){
			STRATEGY_ID = args[1];
			System.out.println("STRATEGY ID : " + STRATEGY_ID);

			validFlag = true;

			String sql="SELECT SID,DID FROM TBL_STRATEGY WHERE STRATEGY_ID=" + STRATEGY_ID;
			Statement mysqlStatement = mysqlConn.createStatement();
			ResultSet rs=mysqlStatement.executeQuery(sql);
			if(rs.next()){
				SOURCE_ID = rs.getString("SID");				//源库id
				DEST_ID = rs.getString("DID");					//目标库id
			}
			rs.close();
			System.out.println("SOURCE ID : " + SOURCE_ID);
			System.out.println("DEST ID : " + DEST_ID);

			//初始化目标库连接start
			sql="SELECT DIP,PORT,USER_PWD_LIST,INSTANCENAME FROM TBL_DEST_DB WHERE DID=" + DEST_ID;
			rs=mysqlStatement.executeQuery(sql);
			if(rs.next()){
				DEST_IP=rs.getString("DIP");					//目标库IP
				DEST_PORT=rs.getString("PORT");
				DEST_PWD=rs.getString("USER_PWD_LIST");			//密码
				DEST_INSTANCE=rs.getString("INSTANCENAME");
			}
			rs.close();
			mysqlStatement.close();

			DEST_URL = "jdbc:oracle:thin:@"+DEST_IP+":"+DEST_PORT+":"+DEST_INSTANCE;	//目标库URL
			System.out.println("DEST URL : " + DEST_URL);

			Driver driver = (Driver) Class.forName("oracle.jdbc.driver.OracleDriver").newInstance();
			DriverManager.registerDriver(driver);
			destConn = DriverManager.getConnection(DEST_URL, DEST_USER, DEST_PWD);
			destConn.setAutoCommit(false);
			//初始化目标库连接end
		}

		if("getsourcescn".equalsIgnoreCase(funcType) || "waittxdone".equalsIgnoreCase(funcType)
			|| "getblobtable".equalsIgnoreCase(funcType) || "createmvlog".equalsIgnoreCase(funcType)
			|| "sourcemv".equalsIgnoreCase(funcType)){
			SOURCE_ID = args[1];
			System.out.println("SOURCE ID : " + SOURCE_ID);

			validFlag = true;
		}

		if(!validFlag){
			System.out.println("arguments not supported!");
			return;
		}

		//初始化源库连接start
		String sql="SELECT SIP,PORT,USERNAME,PASSWORD,INSTANCENAME FROM TBL_SOURCE_DB WHERE SID=" + SOURCE_ID;
		Statement mysqlStatement = mysqlConn.createStatement();
		ResultSet rs=mysqlStatement.executeQuery(sql);
		if(rs.next()){
			SOURCE_IP=rs.getString("sip");					//源库IP
			SOURCE_PORT=rs.getString("PORT");
			SOURCE_USER=rs.getString("username");
			SOURCE_PWD=rs.getString("password");
			SOURCE_INSTANCE=rs.getString("instancename");
		}
		rs.close();
		mysqlStatement.close();

		SOURCE_URL = "jdbc:oracle:thin:@"+SOURCE_IP+":1521:"+SOURCE_INSTANCE;		//源库URL
		System.out.println("SOURCE URL : " + SOURCE_URL);

		Driver driver = (Driver) Class.forName("oracle.jdbc.driver.OracleDriver").newInstance();
		DriverManager.registerDriver(driver);
		sourceConn = DriverManager.getConnection(SOURCE_URL, SOURCE_USER, SOURCE_PWD);
		sourceConn.setAutoCommit(false);
		//初始化源库连接end

		if("getsourcescn".equalsIgnoreCase(funcType)){
			System.out.println();
			GetCurrentScn(sourceConn);
		}

		if("waittxdone".equalsIgnoreCase(funcType)){
			System.out.println();
			WaitPendingtransDone(sourceConn);
		}

		if("getblobtable".equalsIgnoreCase(funcType)){
			System.out.println();
			GetBlobTableNameList(sourceConn, SOURCE_ID);
		}

		if("createmvlog".equalsIgnoreCase(funcType)){
			System.out.println();
			CreateMViewLog(sourceConn, SOURCE_ID);
		}

		if("sourcemv".equalsIgnoreCase(funcType)){
			System.out.println();
			GetBlobTableNameList(sourceConn, SOURCE_ID);
			System.out.println();
			CreateMViewLog(sourceConn, SOURCE_ID);
		}

		if("createdblink".equalsIgnoreCase(funcType)){
			System.out.println();
			CreateDbLink(destConn);
		}

		if("renamemvtable".equalsIgnoreCase(funcType)){
			System.out.println();
			RenameMViewTable(destConn, SOURCE_ID);
		}

		if("createmv".equalsIgnoreCase(funcType)){
			System.out.println();
			CreateMView(destConn, SOURCE_ID);
		}

		if("destmv".equalsIgnoreCase(funcType)){
			System.out.println();
			CreateDbLink(destConn);
			System.out.println();
			RenameMViewTable(destConn, SOURCE_ID);
			System.out.println();
			CreateMView(destConn, SOURCE_ID);
		}

		if("mv2table".equalsIgnoreCase(funcType)){
			System.out.println();
			MV2Table(destConn, SOURCE_ID);
		}
	}

	//获取oracle库当前SCN
	public static void GetCurrentScn(Connection conn) throws Exception {
		String currentScn = get_string(conn, "select dbms_flashback.get_system_change_number as currentscn from dual", "currentscn");
		System.out.println("CURRENT SCN : " + currentScn);
	}

	//等待oracle库当前未完成事务结束
	public static void WaitPendingtransDone(Connection conn) throws Exception {
		String currentScn = get_string(conn, "select dbms_flashback.get_system_change_number as currentscn from dual", "currentscn");
		System.out.println("CURRENT SCN : " + currentScn);

		ArrayList arrayList = new ArrayList();
		String sql="select START_SCN,XIDUSN,XIDSLOT,XIDSQN from v$transaction order by START_SCN";
		Statement statement = conn.createStatement();
		ResultSet rs = statement.executeQuery(sql);
		while(rs.next()){
			arrayList.add(new PendingTran(rs.getObject(1)+"", rs.getObject(2)+"", rs.getObject(3)+"", rs.getObject(4)+""));
			System.out.println("PENDING TRAN : SCN:" + rs.getObject(1) + " " + rs.getObject(2) + " " + rs.getObject(3) + " " + rs.getObject(4));
		}
		rs.close();

		if(arrayList.size() == 0){
			System.out.println("NO PENDING TRAN.");
		}
		else{
			while(true){
				Thread.sleep(10000);	//10seconds
				boolean found=false;
				rs = statement.executeQuery(sql);
				while(rs.next()){
					String XIDUSN = rs.getObject(2)+"";
					String XIDSLT = rs.getObject(3)+"";
					String XIDSQN = rs.getObject(4)+"";
//					System.out.println("[DEBUG]in resultset:"+XIDUSN+" "+XIDSLT+" "+XIDSQN);
					for (int i = 0; i < arrayList.size(); i++) {
						PendingTran r = (PendingTran)arrayList.get(i);
//						System.out.println("[DEBUG]in record:"+r.XIDUSN+" "+r.XIDSLT+" "+r.XIDSQN);
						if(r.XIDUSN.equals(XIDUSN) && r.XIDSLT.equals(XIDSLT) && r.XIDSQN.equals(XIDSQN)){
							found = true;
							break;
						}
					}
					if(found == true) break;
				}
				rs.close();
				if(found == false) break;
			}
			System.out.println("PENDING TRANS ENDED.");
		}
		statement.close();

		currentScn = get_string(conn, "select dbms_flashback.get_system_change_number as currentscn from dual", "currentscn");
		System.out.println("CURRENT SCN : " + currentScn);
	}

	//获取oracle库中含BLOB字段的表名,存入中间库的TBL_BLOBTABLE表中
	public static void GetBlobTableNameList(Connection conn, String sourceid) throws Exception{
		String excludeOwner = "'SYS','SYSTEM','UNKNOWN','MGMT_VIEW','DBSNMP','SYSMAN','LOGMNR','OUTLN','FLOWS_FILES','MDSYS','ORDSYS','EXFSYS','WMSYS','APPQOSSYS','APEX_030200','OWBSYS_AUDIT','ORDDATA','CTXSYS','ANONYMOUS','XDB','ORDPLUGINS','OWBSYS','SI_INFORMTN_SCHEMA','OLAPSYS','ORACLE_OCM','XS$NULL','MDDATA','DIP','APEX_PUBLIC_USER','SPATIAL_CSW_ADMIN_USR','SPATIAL_WFS_ADMIN_USR','SH','PM'";
		String sql="select owner,table_name from DBA_TAB_COLUMNS where owner not in(" + excludeOwner + ") and data_type='BLOB'group by owner,table_name";
		Statement statement = conn.createStatement();
		ResultSet rs = statement.executeQuery(sql);

		Statement mysqlStatement = mysqlConn.createStatement();
		mysqlStatement.executeUpdate("CREATE TABLE IF NOT EXISTS TBL_BLOBTABLE(`SOURCEID` INT NOT NULL,`OWNER` VARCHAR(30) CHARACTER SET utf8 NOT NULL, `TABLE_NAME` VARCHAR(30) CHARACTER SET utf8 NOT NULL)");
		mysqlStatement.executeUpdate("DELETE FROM TBL_BLOBTABLE");
		while(rs.next()){
			String OWNER = rs.getObject(1)+"";
			String TABLE_NAME = rs.getObject(2)+"";
			System.out.println("[OWNER]"+OWNER+"  [TABLE NAME]"+TABLE_NAME);
			mysqlStatement.executeUpdate("INSERT INTO TBL_BLOBTABLE(SOURCEID,OWNER,TABLE_NAME) VALUES("+sourceid+",'"+OWNER+"','"+TABLE_NAME+"')");
		}
		mysqlConn.commit();
		rs.close();
		statement.close();
	}

	//在源库中创建物化视图日志
	public static void CreateMViewLog(Connection conn, String sourceid) throws Exception{
		Statement statement = conn.createStatement();

		Statement mysqlStatement = mysqlConn.createStatement();
		ResultSet rs = mysqlStatement.executeQuery("SELECT OWNER,TABLE_NAME FROM TBL_BLOBTABLE WHERE SOURCEID="+sourceid);
		String sql;
		while(rs.next()){
			String OWNER = rs.getObject(1)+"";
			String TABLE_NAME = rs.getObject(2)+"";
			try{
				sql = "CREATE MATERIALIZED VIEW LOG ON " + OWNER + "." + TABLE_NAME + " WITH ROWID";
				System.out.println(sql);
				statement.executeUpdate(sql);
			}
			catch(Exception e){
				//如果物化视图日志已经存在，跳过该语句，只输出错误信息
				System.out.println("error: " + e.getMessage());
			}
		}
		rs.close();
		mysqlStatement.close();
		statement.close();
	}

	//在目标库创建物化视图
	public static void CreateMView(Connection conn, String sourceid) throws Exception{
		Statement statement = conn.createStatement();
		Statement mysqlStatement = mysqlConn.createStatement();

		//赋创建物化视图权限
		ResultSet rs = mysqlStatement.executeQuery("SELECT DISTINCT OWNER FROM TBL_BLOBTABLE WHERE SOURCEID="+sourceid);
		String sql;
		while(rs.next()){
			String OWNER = rs.getObject(1)+"";
			try{
				sql = "GRANT CREATE TABLE TO " + OWNER;
				System.out.println(sql);
				statement.executeUpdate(sql);
			}
			catch(Exception e){
				throw new Exception("grant error: " + e.getMessage());
			}
		}
		rs.close();

		//创建物化视图
		rs = mysqlStatement.executeQuery("SELECT CONCAT(OWNER,'.',TABLE_NAME) FROM TBL_BLOBTABLE WHERE SOURCEID="+sourceid);
		while(rs.next()){
			String TABLE_NAME = rs.getObject(1)+"";
			try{
				sql = "CREATE MATERIALIZED VIEW " + TABLE_NAME + " REFRESH FAST WITH ROWID " 
					+" AS SELECT * FROM " + TABLE_NAME + "@zwlink";
				System.out.println(sql);
				statement.executeUpdate(sql);
			}
			catch(Exception e){
				//如果物化视图已经存在，跳过该语句，只输出错误信息
				System.out.println("error: " + e.getMessage());
			}
		}
		rs.close();
		mysqlStatement.close();
		statement.close();
	}

	//目标库修改表名
	public static void RenameMViewTable(Connection conn, String sourceid) throws Exception{
		Statement statement = conn.createStatement();

		Statement mysqlStatement = mysqlConn.createStatement();
		ResultSet rs = mysqlStatement.executeQuery("SELECT OWNER,TABLE_NAME FROM TBL_BLOBTABLE WHERE SOURCEID="+sourceid);
		String sql;

		while(rs.next()){
			String OWNER = rs.getObject(1)+"";
			String TABLE_NAME = rs.getObject(2)+"";
			String TMPTABLE_NAME = TABLE_NAME+"_TMP";
			try{
				sql = "ALTER TABLE " + OWNER + "." + TABLE_NAME + " RENAME TO " + TMPTABLE_NAME;
				System.out.println(sql);
				statement.executeUpdate(sql);
			}
			catch(Exception e){
				System.out.println("error: " + e.getMessage());
			}
		}
		rs.close();
		mysqlStatement.close();
		statement.close();
	}

	//目标库把物化视图数据导入表中
	public static void MV2Table(Connection conn, String sourceid) throws Exception{
		Statement statement = conn.createStatement();

		Statement mysqlStatement = mysqlConn.createStatement();
		ResultSet rs = mysqlStatement.executeQuery("SELECT OWNER,TABLE_NAME FROM TBL_BLOBTABLE WHERE SOURCEID="+sourceid);
		String sql;

		while(rs.next()){
			String OWNER = rs.getObject(1)+"";
			String TABLE_NAME = rs.getObject(2)+"";
			String TMPTABLE_NAME = TABLE_NAME+"_TMP";
			try{
				sql = "TRUNCATE TABLE " + OWNER + "." + TMPTABLE_NAME;
				System.out.println(sql);
				statement.executeUpdate(sql);

				sql = "INSERT INTO " + OWNER + "." + TMPTABLE_NAME + " SELECT * FROM " + OWNER + "." + TABLE_NAME;
				System.out.println(sql);
				statement.executeUpdate(sql);

				sql = "DROP MATERIALIZED VIEW " + OWNER + "." + TABLE_NAME;
				System.out.println(sql);
				statement.executeUpdate(sql);

				sql = "ALTER TABLE " + OWNER + "." + TMPTABLE_NAME + " RENAME TO " + TABLE_NAME;
				System.out.println(sql);
				statement.executeUpdate(sql);
			}
			catch(Exception e){
				System.out.println("error: " + e.getMessage());
			}
		}

		rs.close();
		mysqlStatement.close();
		statement.close();
	}

	public static void CreateDbLink(Connection conn) throws Exception{
//		create public database link zwlink connect to system identified by oracle
//		using '(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=192.168.20.144)(PORT=1521))(CONNECT_DATA=(SERVER=DEDICATED)(SERVICE_NAME=orcl)))';
		String sql = "CREATE PUBLIC DATABASE LINK ZWLINK CONNECT TO "+SOURCE_USER+" IDENTIFIED BY "+SOURCE_PWD+
				" USING '(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST="+SOURCE_IP+")(PORT="+SOURCE_PORT+"))(CONNECT_DATA=(SERVER=DEDICATED)(SERVICE_NAME="+SOURCE_INSTANCE+")))'";
		System.out.println(sql);
		try{
			Statement statement = conn.createStatement();
			statement.executeUpdate(sql);
			statement.close();
		}
		catch(Exception e){
			System.out.println("error: " + e.getMessage());
		}
	}
}

class PendingTran{
	PendingTran(String START_SCN,String XIDUSN,String XIDSLT,String XIDSQN){
		this.START_SCN = START_SCN;
		this.XIDUSN = XIDUSN;
		this.XIDSLT = XIDSLT;
		this.XIDSQN = XIDSQN;
	}

	String START_SCN;
	String XIDUSN;
	String XIDSLT;
	String XIDSQN;
}