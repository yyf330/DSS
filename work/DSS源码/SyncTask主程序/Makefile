#### 由所有生成目标共享的编译器和工具定义 #####
CCC = g++
CXX = g++
BASICOPTS = -g -m64
CCFLAGS = $(BASICOPTS)
CXXFLAGS = $(BASICOPTS)
CCADMIN = 


# 定义目标目录。
TARGETDIR_SyncTask=GNU-i386-Linux


all: $(TARGETDIR_SyncTask)/SyncTask

## 目标： SyncTask
OBJS_SyncTask =  \
	$(TARGETDIR_SyncTask)/task.o \
	$(TARGETDIR_SyncTask)/taskqueue.o \
	$(TARGETDIR_SyncTask)/threadstate.o \
	$(TARGETDIR_SyncTask)/threadstatecollection.o \
	$(TARGETDIR_SyncTask)/socketex.o \
	$(TARGETDIR_SyncTask)/socketexnb.o \
	$(TARGETDIR_SyncTask)/networkmanager.o \
	$(TARGETDIR_SyncTask)/beserror.o \
	$(TARGETDIR_SyncTask)/taskprocdesc.o \
	$(TARGETDIR_SyncTask)/beslog.o \
	$(TARGETDIR_SyncTask)/dbdatamanager.o \
	$(TARGETDIR_SyncTask)/begenerallib.o \
	$(TARGETDIR_SyncTask)/becommand.o \
	$(TARGETDIR_SyncTask)/commandunit.o \
	$(TARGETDIR_SyncTask)/inifile.o \
	$(TARGETDIR_SyncTask)/smsmanager.o \
	$(TARGETDIR_SyncTask)/dralarm.o \
	$(TARGETDIR_SyncTask)/alarmmanager.o \
	$(TARGETDIR_SyncTask)/mailmanager.o \
	$(TARGETDIR_SyncTask)/synctask.o
CPPFLAGS_SyncTask = \
		-I/usr/include/libxml2 \
		-I/usr/include
SYSLIBS_SyncTask = -lm -lpthread -lmysqlcppconn -lcrypto  -L/usr/lib -lxml2 -lz -lm -lzwlicensef -lzwlicensed libdog_linux_x86_64_3147187.a -lrt -ldl
USERLIBS_SyncTask = $(SYSLIBS_SyncTask) 
DEPLIBS_SyncTask =  
LDLIBS_SyncTask = $(USERLIBS_SyncTask) 


# 链接或归档
$(TARGETDIR_SyncTask)/SyncTask: $(TARGETDIR_SyncTask) $(OBJS_SyncTask) $(DEPLIBS_SyncTask)
	$(LINK.cc) $(CCFLAGS_SyncTask) $(CPPFLAGS_SyncTask) -o $@ $(OBJS_SyncTask) $(LDLIBS_SyncTask)


# 将源文件编译为 .o 文件
$(TARGETDIR_SyncTask)/task.o: $(TARGETDIR_SyncTask) task.cpp
	$(COMPILE.cc) $(CCFLAGS_SyncTask) $(CPPFLAGS_SyncTask) -o $@ task.cpp

$(TARGETDIR_SyncTask)/taskqueue.o: $(TARGETDIR_SyncTask) taskqueue.cpp
	$(COMPILE.cc) $(CCFLAGS_SyncTask) $(CPPFLAGS_SyncTask) -o $@ taskqueue.cpp

$(TARGETDIR_SyncTask)/threadstate.o: $(TARGETDIR_SyncTask) threadstate.cpp
	$(COMPILE.cc) $(CCFLAGS_SyncTask) $(CPPFLAGS_SyncTask) -o $@ threadstate.cpp

$(TARGETDIR_SyncTask)/threadstatecollection.o: $(TARGETDIR_SyncTask) threadstatecollection.cpp
	$(COMPILE.cc) $(CCFLAGS_SyncTask) $(CPPFLAGS_SyncTask) -o $@ threadstatecollection.cpp

$(TARGETDIR_SyncTask)/socketex.o: $(TARGETDIR_SyncTask) socketex.cpp
	$(COMPILE.cc) $(CCFLAGS_SyncTask) $(CPPFLAGS_SyncTask) -o $@ socketex.cpp

$(TARGETDIR_SyncTask)/socketexnb.o: $(TARGETDIR_SyncTask) socketexnb.cpp
	$(COMPILE.cc) $(CCFLAGS_SyncTask) $(CPPFLAGS_SyncTask) -o $@ socketexnb.cpp

$(TARGETDIR_SyncTask)/networkmanager.o: $(TARGETDIR_SyncTask) networkmanager.cpp
	$(COMPILE.cc) $(CCFLAGS_SyncTask) $(CPPFLAGS_SyncTask) -o $@ networkmanager.cpp

$(TARGETDIR_SyncTask)/beserror.o: $(TARGETDIR_SyncTask) beserror.cpp
	$(COMPILE.cc) $(CCFLAGS_SyncTask) $(CPPFLAGS_SyncTask) -o $@ beserror.cpp

$(TARGETDIR_SyncTask)/taskprocdesc.o: $(TARGETDIR_SyncTask) taskprocdesc.cpp
	$(COMPILE.cc) $(CCFLAGS_SyncTask) $(CPPFLAGS_SyncTask) -o $@ taskprocdesc.cpp

$(TARGETDIR_SyncTask)/beslog.o: $(TARGETDIR_SyncTask) beslog.cpp
	$(COMPILE.cc) $(CCFLAGS_SyncTask) $(CPPFLAGS_SyncTask) -o $@ beslog.cpp

$(TARGETDIR_SyncTask)/dbdatamanager.o: $(TARGETDIR_SyncTask) dbdatamanager.cpp
	$(COMPILE.cc) $(CCFLAGS_SyncTask) $(CPPFLAGS_SyncTask) -o $@ dbdatamanager.cpp

$(TARGETDIR_SyncTask)/begenerallib.o: $(TARGETDIR_SyncTask) begenerallib.cpp
	$(COMPILE.cc) $(CCFLAGS_SyncTask) $(CPPFLAGS_SyncTask) -o $@ begenerallib.cpp

$(TARGETDIR_SyncTask)/becommand.o: $(TARGETDIR_SyncTask) becommand.cpp
	$(COMPILE.cc) $(CCFLAGS_SyncTask) $(CPPFLAGS_SyncTask) -o $@ becommand.cpp

$(TARGETDIR_SyncTask)/commandunit.o: $(TARGETDIR_SyncTask) commandunit.cpp
	$(COMPILE.cc) $(CCFLAGS_SyncTask) $(CPPFLAGS_SyncTask) -o $@ commandunit.cpp

$(TARGETDIR_SyncTask)/inifile.o: $(TARGETDIR_SyncTask) inifile.cpp
	$(COMPILE.cc) $(CCFLAGS_SyncTask) $(CPPFLAGS_SyncTask) -o $@ inifile.cpp

$(TARGETDIR_SyncTask)/smsmanager.o: $(TARGETDIR_SyncTask) smsmanager.cpp
	$(COMPILE.cc) $(CCFLAGS_SyncTask) $(CPPFLAGS_SyncTask) -o $@ smsmanager.cpp

$(TARGETDIR_SyncTask)/dralarm.o: $(TARGETDIR_SyncTask) dralarm.cpp
	$(COMPILE.cc) $(CCFLAGS_SyncTask) $(CPPFLAGS_SyncTask) -o $@ dralarm.cpp

$(TARGETDIR_SyncTask)/alarmmanager.o: $(TARGETDIR_SyncTask) alarmmanager.cpp
	$(COMPILE.cc) $(CCFLAGS_SyncTask) $(CPPFLAGS_SyncTask) -o $@ alarmmanager.cpp

$(TARGETDIR_SyncTask)/mailmanager.o: $(TARGETDIR_SyncTask) mailmanager.cpp
	$(COMPILE.cc) $(CCFLAGS_SyncTask) $(CPPFLAGS_SyncTask) -o $@ mailmanager.cpp

$(TARGETDIR_SyncTask)/synctask.o: $(TARGETDIR_SyncTask) synctask.cpp
	$(COMPILE.cc) $(CCFLAGS_SyncTask) $(CPPFLAGS_SyncTask) -o $@ synctask.cpp

#### 清理目标将会删除所有生成的文件 ####
clean:
	rm -f \
		$(TARGETDIR_SyncTask)/SyncTask \
		$(TARGETDIR_SyncTask)/task.o \
		$(TARGETDIR_SyncTask)/taskqueue.o \
		$(TARGETDIR_SyncTask)/threadstate.o \
		$(TARGETDIR_SyncTask)/threadstatecollection.o \
		$(TARGETDIR_SyncTask)/socketex.o \
		$(TARGETDIR_SyncTask)/socketexnb.o \
		$(TARGETDIR_SyncTask)/networkmanager.o \
		$(TARGETDIR_SyncTask)/beserror.o \
		$(TARGETDIR_SyncTask)/taskprocdesc.o \
		$(TARGETDIR_SyncTask)/beslog.o \
		$(TARGETDIR_SyncTask)/dbdatamanager.o \
		$(TARGETDIR_SyncTask)/begenerallib.o \
		$(TARGETDIR_SyncTask)/becommand.o \
		$(TARGETDIR_SyncTask)/commandunit.o \
		$(TARGETDIR_SyncTask)/inifile.o \
		$(TARGETDIR_SyncTask)/smsmanager.o \
		$(TARGETDIR_SyncTask)/dralarm.o \
		$(TARGETDIR_SyncTask)/alarmmanager.o \
		$(TARGETDIR_SyncTask)/mailmanager.o \
		$(TARGETDIR_SyncTask)/synctask.o
	$(CCADMIN)
	rm -f -r $(TARGETDIR_SyncTask)


# 创建目标目录（如果需要）
$(TARGETDIR_SyncTask):
	mkdir -p $(TARGETDIR_SyncTask)


# 启用依赖关系检查
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-i386-Linux

