!INCLUDE <wcedefs.mak>

CFLAGS=/W3 /Ox /O2 /Ob2 /GF /Gy /Zl /nologo $(WCETARGETDEFS) /DUNICODE /D_UNICODE /DWIN32 /D_USE_32BIT_TIME_T /DWIN32_LEAN_AND_MEAN /Iinclude /D_WINDLL /D_DLL /Foobj/ /Fdlib/wcecompatex.pdb

!IF "$(WCEPLATFORM)"=="MS_POCKET_PC_2000"
CFLAGS=$(CFLAGS) /DWIN32_PLATFORM_PSPC
!ENDIF

!IFDEF DEBUG
CFLAGS=$(CFLAGS) /Zi /DDEBUG /D_DEBUG
!ELSE
CFLAGS=$(CFLAGS) /Zi /DNDEBUG
!ENDIF

!IF "$(MSVS)"=="2008"
CFLAGS=$(CFLAGS) /Zc:wchar_t-,forScope- /GS-
LFLAGS=/DEF:"src\wcecompat.def" /DLL /MACHINE:$(WCELDMACHINE) /SUBSYSTEM:WINDOWSCE,$(WCELDVERSION) /NODEFAULTLIB /DYNAMICBASE /NXCOMPAT
!ELSE
LFLAGS=/DEF:"src\wcecompat.def" /DLL /MACHINE:$(WCELDMACHINE) /SUBSYSTEM:WINDOWSCE,$(WCELDVERSION) /NODEFAULTLIB
!ENDIF

CORELIBS=coredll.lib corelibc.lib ole32.lib oleaut32.lib uuid.lib commctrl.lib

SRC =							\
	src/args.cpp				\
	src/assert.cpp				\
	src/ChildData.cpp			\
	src/env.cpp					\
	src/errno.cpp				\
	src/io.cpp					\
	src/pipe.cpp				\
	src/process.c				\
	src/redir.cpp				\
	src/signal.c				\
	src/stat.cpp				\
	src/stdio_extras.cpp		\
	src/stdlib_extras.cpp		\
	src/string_extras.cpp		\
	src/time_ce.cpp				\
	src/time.cpp				\
	src/timeb.cpp				\
	src/ts_string.cpp			\
	src/winsock_extras.cpp

!IF "$(WCEVERSION)"=="211"
SRC =							\
	$(SRC)						\
	src/wce211_ctype.c			\
	src/wce211_string.c
!ENDIF

OBJS = $(SRC:src=obj)
OBJS = $(OBJS:.cpp=.obj)
OBJS = $(OBJS:.c=.obj)

{src}.c{obj}.obj:
	@$(CC) $(CFLAGS) -c $<

{src}.cpp{obj}.obj:
	@$(CC) $(CFLAGS) -c $<

all: lib\wcecompatex.lib lib\wcecompat.lib lib\winmain.lib
#	echo $(OBJS)

obj:
	@md obj 2> NUL

lib:
	@md lib 2> NUL

$(OBJS): makefile obj

obj/winmain.obj: makefile obj src/winmain.cpp

clean:
	@echo Deleting target libraries...
	@del lib\*.lib
	@echo Deleting object files...
	@del obj\*.obj

lib\wcecompat.lib: lib $(OBJS) makefile
	@link /nologo /out:lib\wcecompat.dll /pdb:lib\wcecompat.pdb $(LFLAGS) $(OBJS) $(CORELIBS)

lib\wcecompatex.lib: lib $(OBJS) obj/winmain.obj makefile
	@lib /nologo /out:lib\wcecompatex.lib $(OBJS) obj/winmain.obj

lib\winmain.lib: lib obj/winmain.obj makefile
	@lib /nologo /out:lib\winmain.lib obj/winmain.obj
