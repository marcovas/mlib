ifndef WINDIR
MODULE		= libmlib.so
else
MODULE		= MLib.dll
endif
STATICMODULE= libmlib.a
OK			= OK

ifdef WINDIR
INSTALL_TARGET	= install_win32
CLEAN_SPECIFIC	= clean_win32 
else
CLEAN_SPECIFIC	= clean_unix
INSTALL_TARGET	= install_unix
endif


CC			= g++
DOXGEN		= doxygen

OPT			= -fomit-frame-pointer -funroll-loops -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE

INCDIR		= ./include
SRCDIR		= ./src
BINDIR		= ./bin
DOCDIR		= ./doc/html

INCFLAGS	= -I$(INCDIR)

ifdef WINDIR
FLAG		= -Wall -Wno-deprecated
LIBS		= -lWs2_32
INCDIR		+= -Ic:\MLibs\include
else
FLAG		= -Wall -Wno-deprecated  -fPIC
LIBS		= -lresolv -lpthread
endif

DEBUG		= -ggdb -O0
NDEBUG		= -O3
LDFLAGS		= -shared

CFLAGS		= $(FLAG) $(INCFLAGS)

SOURCES		= \
			Arguments.cpp  \
			File.cpp  \
			Mutex.cpp  \
			NamedPipe.cpp  \
			PThread.cpp  \
			Semaphore.cpp  \
			Stream.cpp  \
			UnnamedPipe.cpp\
			Socket.cpp\
			TCPSocket.cpp\
			UDPSocket.cpp\
			Timer.cpp\
			Stopwatch.cpp\
			AutoThread.cpp\
			NetworkInterface.cpp\
			#main.cpp
			
HEADERS		= 	Arguments.h  \
			File.h  \
			MpegTimer.h  \
			Mutex.h  \
			NamedPipe.h  \
			PThread.h  \
			Semaphore.h  \
			Stream.h  \
			SyncPoint.h  \
			UnnamedPipe.h\
			Socket.h\
			TCPSocket.h\
			UDPSocket.h\
			Timer.h\
			Stopwatch.h\
			AutoThread.h\
			NetworkInterface.h

SRCS		= $(addprefix $(SRCDIR)/,$(SOURCES))
OBJS		= $(SRCS:.cpp=.o)
EXE			= $(BINDIR)/$(MODULE)
INIT		= $(BINDIR)

.SUFFIXES: .cpp .o .h

all: $(INIT) $(OBJS) $(EXE)

$(INIT):
	@echo -n Criando diretorio bin...
	@mkdir $(BINDIR)
	@echo -e " $(OK)"

$(EXE):
	$(CC) $(LIBS) $(SRCDIR)/*.o -o $(BINDIR)/$(MODULE) $(LDFLAGS) $(LIBS) $(NDEBUG)
	strip $(BINDIR)/$(MODULE)
	@echo -e "    $(OK)"

static: $(INIT) $(OBJS)
	ar rcs $(BINDIR)/$(STATICMODULE) $(SRCDIR)/*.o
	
%.o: %.cpp
	$(CC) $(CFLAGS) $(NDEBUG) -c $< -o $@ 
	
move:
	cp $(SRCDIR)/*.o $(BINDIR)
 
install: all $(INSTALL_TARGET)
	@echo "Install complete"
	
install_win32:
	@echo "Install Win32"
	test -d $(SYSTEMDRIVE)/MLibs/include || mkdir $(SYSTEMDRIVE)/MLibs/include
	cd $(INCDIR); cp $(HEADERS) $(SYSTEMDRIVE)/MLibs/include
	cd $(INCDIR); cp $(HEADERS) $(SYSTEMDRIVE)/mingw/include
	#cp $(BINDIR)/$(MODULE) $(SYSTEMROOT)/system32
	cp $(BINDIR)/$(MODULE) $(SYSTEMDRIVE)/mingw/lib/ 
 
install_unix: $(EXE)
	@echo "Install Unix"
	cp $(BINDIR)/$(MODULE) /usr/lib
	(test -f $(BINDIR)/$(STATICMODULE) && cp $(BINDIR)/$(STATICMODULE) /usr/lib) || test -d /tmp
	cp include/*.h /usr/local/include
	cd /usr/local/include ;	chmod 644 $(HEADERS)
	ldconfig -v -n /usr/lib>/dev/null

clean-all: clean $(CLEAN_SPECIFIC)

clean:
	rm -rf $(BINDIR)
	rm -f $(SRCDIR)/*.o
	
	
clean_win32:
	cd $(SYSTEMDRIVE)/MLibs/include; rm $(HEADERS)
	cd $(SYSTEMDRIVE)/mingw/include; rm $(HEADERS)
	rm $(SYSTEMROOT)/system32/$(MODULE)
	rm $(SYSTEMDRIVE)/mingw/lib/$(MODULE)

clean_unix:
	cd /usr/local/include; rm $(HEADERS)
	rm /usr/local/lib/$(MODULE)
	ldconfig
  
