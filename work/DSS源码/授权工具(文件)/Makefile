## -*- Makefile -*-
##
## 用户： root
## 时间： 2010-5-27 21:39:47
## 由 Sun Studio 创建的 Makefile。
##
## 此文件是自动生成的。
##


#### 由所有生成目标共享的编译器和工具定义 #####
CCC = g++
CXX = g++
BASICOPTS = -g
CCFLAGS = $(BASICOPTS)
CXXFLAGS = $(BASICOPTS)
CCADMIN = 


# 定义目标目录。
TARGETDIR_DssGenKey=GNU-i386-Linux

all: $(TARGETDIR_DssGenKey)/DssGenKey

## 目标： DssGenKey
OBJS_DssGenKey =  \
	$(TARGETDIR_DssGenKey)/gencrypt.o \
	$(TARGETDIR_DssGenKey)/begenerallib.o \
	$(TARGETDIR_DssGenKey)/dssgenkey.o
SYSLIBS_DssGenKey = -lpthread -lcrypto -lm -L. -lzwlicense
USERLIBS_DssGenKey = $(SYSLIBS_DssGenKey) 
DEPLIBS_DssGenKey =  
LDLIBS_DssGenKey = $(USERLIBS_DssGenKey)


# 链接或归档
$(TARGETDIR_DssGenKey)/DssGenKey: $(TARGETDIR_DssGenKey) $(OBJS_DssGenKey) $(DEPLIBS_DssGenKey)
	$(LINK.cc) $(CCFLAGS_DssGenKey) $(CPPFLAGS_DssGenKey) -o $@ $(OBJS_DssGenKey) $(LDLIBS_DssGenKey)


# 将源文件编译为 .o 文件
$(TARGETDIR_DssGenKey)/gencrypt.o: $(TARGETDIR_DssGenKey) gencrypt.cpp
	$(COMPILE.cc) $(CCFLAGS_DssGenKey) $(CPPFLAGS_DssGenKey) -o $@ gencrypt.cpp

$(TARGETDIR_DssGenKey)/begenerallib.o: $(TARGETDIR_DssGenKey) begenerallib.cpp
	$(COMPILE.cc) $(CCFLAGS_DssGenKey) $(CPPFLAGS_DssGenKey) -o $@ begenerallib.cpp

$(TARGETDIR_DssGenKey)/dssgenkey.o: $(TARGETDIR_DssGenKey) dssgenkey.cpp
	$(COMPILE.cc) $(CCFLAGS_DssGenKey) $(CPPFLAGS_DssGenKey) -o $@ dssgenkey.cpp



#### 清理目标将会删除所有生成的文件 ####
clean:
	rm -f \
		$(TARGETDIR_DssGenKey)/DssGenKey \
		$(TARGETDIR_DssGenKey)/gencrypt.o \
		$(TARGETDIR_DssGenKey)/begenerallib.o \
		$(TARGETDIR_DssGenKey)/dssgenkey.o
	$(CCADMIN)
	rm -f -r $(TARGETDIR_DssGenKey)


# 创建目标目录（如果需要）
$(TARGETDIR_DssGenKey):
	mkdir -p $(TARGETDIR_DssGenKey)


# 启用依赖关系检查
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-i386-Linux

