
PROJECT_MODULES = src/lld_encoder.c           \
	          	  src/lld_control.c			  \
		          src/debug.c	          	  \
                  scr/lld_charge_level.c      \
                  scr/lld_steering_control.c  \
                  src/control_system.c        \
                  src/odometry.c		      \


PROJECT_TESTS   = tests/test_lld_encoder.c         	\
		          tests/test_lld_control.c         	\
                  tests/test_lld_charge_level.c    	\
                  tests/test_lld_steering_control.c \
                  tests/test_steering_system.c		\
                  tests/test_odometry.c			    \
                  tests/test_driving_system.c       \
                                           
PROJECT_CSRC    = src/main.c src/common.c \
    				$(PROJECT_MODULES) $(PROJECT_TESTS)


PROJECT_CPPSRC 	= 

PROJECT_INCDIR	= include tests $(ROSINC)

PROJECT_LIBS	=  -lm

