SUBDIR = src

.PHONY : all $(SUBDIR) clean

all : $(SUBDIR)

$(SUBDIR) : 
	$(MAKE) -C $@

clean : 
	$(MAKE) clean -C $(SUBDIR)
