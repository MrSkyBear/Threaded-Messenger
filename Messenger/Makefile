# Makefile for socket examples

CXX=		g++ $(CCFLAGS)
MSGD=		msgd.o inet-server.o server.o Post_Office.o Worker.o Messenger.o Server_Message.o 
MSG=		msg.o inet-client.o client.o
OBJS =		$(MSGD) $(MSG) 

LIBS= -pthread

CCFLAGS= -g

all:	msgd msg

msgd:$(MSGD)
	$(CXX) -o msgd $(MSGD) $(LIBS)

msg:$(MSG)
	$(CXX) -o msg $(MSG) $(LIBS)

clean:
	rm -f $(OBJS) $(OBJS:.o=.d)

realclean:
	rm -f $(OBJS) $(OBJS:.o=.d) msgd msg


# These lines ensure that dependencies are handled automatically.
%.d:	%.cc
	$(SHELL) -ec '$(CC) -M $(CPPFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'

include	$(OBJS:.o=.d)
