CC     = gcc
CPARAM = -Wall -pedantic

SRCDIR = src
BLDDIR = build
LIBDIR = /usr/lib
HEDDIR = /usr/include

LIBS := libhacal_asm

clean:
	rm -rf $(BLDDIR)

all: $(BLDDIR)/ $(foreach lib,$(LIBS),$(lib).so)
	@echo
	@echo Build done!

install: $(LIBDIR)/ $(HEDDIR)/
	$(foreach lib,$(LIBS),cp $(BLDDIR)/$(lib).so $(LIBDIR)/)
	$(foreach lib,$(LIBS),cp $(SRCDIR)/$(lib).h $(HEDDIR)/)
	ldconfig -n /usr/lib
	@echo
	@echo Install done!

uninstall: 
	$(foreach lib,$(LIBS),rm -f $(LIBDIR)/$(lib).so)
	$(foreach lib,$(LIBS),rm -f $(HEDDIR)/$(lib).h)

%.so: $(SRCDIR)/%.c
	$(CC) $(CPARAM) -shared -o $(BLDDIR)/$@ -fPIC $<

%.h: $(SRCDIR)/%.c
	cp $< $(BLDDIR)/$@

%/:
	mkdir $@
