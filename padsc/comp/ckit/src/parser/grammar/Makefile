CPP = /lib/cpp
#CPP = /usr/ccs/lib/cpp

all: d.grm c.grm

d.grm: cd.grm; $(CPP)  -DDPARSE -P cd.grm d.grm

c.grm: cd.grm; $(CPP) -P cd.grm c.grm

clean:
	rm c.grm.desc c.grm.sig c.grm.sml c.lex.sml 
