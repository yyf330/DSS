## -*- Makefile -*-
##
## 用户： root
## 时间： 2015-8-21 10:13:38
## Makefile created by Oracle Solaris Studio.
##
## 此文件是自动生成的。
##


#### 由所有生成目标共享的编译器和工具定义 #####
CCC = g++
CXX = g++
BASICOPTS = -g -m64 -fPIC
CCFLAGS = $(BASICOPTS)
CXXFLAGS = $(BASICOPTS)
CCADMIN = 


# 定义目标目录。
TARGETDIR_libzwlicensed.so=GNU-amd64-Linux


all: $(TARGETDIR_libzwlicensed.so)/libzwlicensed.so

## 目标： libzwlicensed.so
CCFLAGS_libzwlicensed.so = 
OBJS_libzwlicensed.so =  \
	$(TARGETDIR_libzwlicensed.so)/besencryptd.o


# 链接或归档
SHAREDLIB_FLAGS_libzwlicensed.so = -shared 
$(TARGETDIR_libzwlicensed.so)/libzwlicensed.so: $(TARGETDIR_libzwlicensed.so) $(OBJS_libzwlicensed.so) $(DEPLIBS_libzwlicensed.so)
	$(LINK.cc) $(CCFLAGS_libzwlicensed.so) $(CPPFLAGS_libzwlicensed.so) -o $@ $(OBJS_libzwlicensed.so) $(SHAREDLIB_FLAGS_libzwlicensed.so) $(LDLIBS_libzwlicensed.so)


# 将源文件编译为 .o 文件

$(TARGETDIR_libzwlicensed.so)/besencryptd.o: $(TARGETDIR_libzwlicensed.so) besencryptd.cpp
	$(COMPILE.cc) $(CCFLAGS_libzwlicensed.so) $(CPPFLAGS_libzwlicensed.so) -o $@ besencryptd.cpp



#### 清理目标将会删除所有生成的文件 ####
clean:
	rm -f \
		$(TARGETDIR_libzwlicensed.so)/libzwlicensed.so \
		$(TARGETDIR_libzwlicensed.so)/besencryptd.o
	$(CCADMIN)
	rm -f -r $(TARGETDIR_libzwlicensed.so)


# 创建目标目录（如果需要）
$(TARGETDIR_libzwlicensed.so):
	mkdir -p $(TARGETDIR_libzwlicensed.so)


# 启用依赖关系检查
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Linux

