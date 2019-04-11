

PROJECT_MODULES = src/lld_steering_control.c  	\
                  src/lld_control.c  			\
                  src/debug.c        
				 

PROJECT_TESTS   = tests/test_lld_steering_control.c \
                  tests/test_lld_control.c          \
                  tests/test_lld_calibration.c

PROJECT_CSRC    = src/main.c src/common.c \
    				$(PROJECT_MODULES) $(PROJECT_TESTS)


PROJECT_CPPSRC 	= 

PROJECT_INCDIR	= include tests $(ROSINC)

PROJECT_LIBS	=  -lm

