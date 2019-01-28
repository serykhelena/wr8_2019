

PROJECT_MODULES = src/lld_control.c        		\
				  src/odometry.c				\	

PROJECT_TESTS   = tests/test_lld_control.c     	\

PROJECT_CSRC    = src/main.c src/common.c \
    				$(PROJECT_MODULES) $(PROJECT_TESTS)


PROJECT_CPPSRC 	= 

PROJECT_INCDIR	= include tests $(ROSINC)

PROJECT_LIBS	=  -lm

