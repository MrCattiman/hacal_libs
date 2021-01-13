CC     = gcc
CPARAM = -Wall -pedantic

SRCDIR = src
BLDDIR = build
LIBDIR = /usr/lib/HACAL
HEDDIR = /usr/include/HACAL

LIBS := hacal_asmlib

clean:
	rm -rf $(BLDDIR)

all: $(BLDDIR)/ $(foreach lib,$(LIBS),$(lib).so)
	@echo
	@echo Build done!

install: 
	$(foreach exe,$(EXES),cp $(BLDDIR)/$(exe) $(EXEDIR)/)
	@echo
	@echo Install done!

uninstall: 
	rm -rvf $(LIBDIR)
	rm -rvf $(HEDDIR)

%.so: $(SRCDIR)/%.c
	$(CC) $(CPARAM) -shared -o $(BLDDIR)/$@ -fPIC $<

%.h: $(SRCDIR)/%.c
	cp $< $(BLDDIR)/$@

%/:
	mkdir $@
