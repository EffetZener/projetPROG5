CC=$(GCC_CROSS)
CCAS=$(GCC_CROSS)
AM_CFLAGS=-mbig-endian
AM_CCASFLAGS=-mbig-endian
AM_LDFLAGS=-nostdlib -T $(srcdir)/linker_script -n --entry main
AM_LDFLAGS+=-Wl,-EB

if HAVE_ARM_COMPILER
noinst_PROGRAMS=example1 example2 example3 example4 example5 example_add example_shift example_load_store example_multiple_load_store example_branch example_test example_mrs insertion_sort

all-am: $(PROGS)
endif

example1_SOURCES=example1.s
example2_SOURCES=example2.s
example3_SOURCES=example3.s
example4_SOURCES=example4.s
example5_SOURCES=example5.s
example_add_SOURCES=example_add.s
example_shift_SOURCES=example_shift.s
example_load_store_SOURCES=example_load_store.s
example_multiple_load_store_SOURCES=example_multiple_load_store.s
example_branch_SOURCES=example_branch.s
example_test_SOURCES=example_test.s
example_mrs_SOURCES=example_mrs.s
insertion_sort_SOURCES=insertion_sort.c

EXTRA_DIST=linker_script

clean-local:
	$(RM) $(PROGS)
