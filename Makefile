include Makefile.tracer

all: trace

.PHONY: trace
trace:
	$(call git_commit, "make invoked")	
