CC      = g++
PROF    = -O -g
NOCRYPT =
C_FLAGS = -Wall $(PROF) $(NOCRYPT)
L_FLAGS =  $(PROF)

O_FILES = act_comm.o act_hera.o act_info.o act_move.o act_obj.o act_wiz.o \
          ban.o comm.o const.o db.o db2.o dilek.o effects.o ek.o ek2.o fight.o flags.o handler.o \
          healer.o interp.o kumar.c lookup.o mafya.o magic.o magic2.o martial_art.o \
          mob_prog.o obj_prog.o quest.o recycle.o save.o  skills.o \
          special.o tables.o update.o -lcrypt

anatolia: $(O_FILES)
	rm -f uzakdiyarlar
	$(CC) $(L_FLAGS) -o uzakdiyarlar $(O_FILES)

.c.o: merc.h
	$(CC) -c $(C_FLAGS) $<

clean:
	@rm -f *.o
	make
