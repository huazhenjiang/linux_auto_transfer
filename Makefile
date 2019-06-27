PROJ_ROOT        := $(shell pwd)
INC = -I $(PROJ_ROOT)/include
LIB = -L $(PROJ_ROOT)/lib
CFLAGS += -Wall -Wextra
BUILD_DIR = $(PROJ_ROOT)/output
FINAL_TARGET=$(BUILD_DIR)/auto_transfer


#i=0
#FUN_file= file 
export PROJ_ROOT INC LIB CFLAGS BUILD_DIR FINAL_TARGET



objects := $(patsubst %.c, %.o,$(wildcard *.c))
#objects := $(patsubst $(SOURCE_C)/%.c, $(SOURCE_C)/%.o,$(wildcard $(SOURCE_C)/*.c))

HTML_folder = $(PROJ_ROOT)/output/html
CSS_folder = $(PROJ_ROOT)/output/css
JS_folder = $(PROJ_ROOT)/output/js

target_html := $(wildcard $(HTML_folder)/*.html)
target_css := $(wildcard $(CSS_folder)/*.css)
target_js := $(wildcard $(JS_folder)/*.js)

c_file_name ?= $(wildcard $(PROJ_ROOT)/output/out/*.c)
filter_path_name ?= $(notdir $(c_file_name))
filter_suffix_name ?= $(basename $(filter_path_name))

AUTO_trans: $(FINAL_TARGET) trans_html trans_css trans_js produce_stringweb_h


produce_stringweb_h:
	@echo "$(c_file_name)\n"
	@echo "$(filter_path_name)\n"
	@echo "$(filter_suffix_name)\n"
	for number in $(filter_suffix_name) ; do \
		echo "extern char *$$number;" >> $(PROJ_ROOT)/output/out/stringweb.h ; \
	done
trans_html:
	for number in $(target_html) ; do \
		cd $(BUILD_DIR) && pwd && ./auto_transfer $$number ; \
	done
trans_css:
	for number in $(target_css) ; do \
		cd $(BUILD_DIR) && pwd && ./auto_transfer $$number ; \
	done
trans_js:
	for number in $(target_js) ; do \
		cd $(BUILD_DIR) && pwd && ./auto_transfer $$number ; \
	done


$(FINAL_TARGET): init $(objects)
	cc $(CFLAGS) $(INC) -o $(FINAL_TARGET) $(objects)
init:
	mkdir -p $(BUILD_DIR)


.PHONY: clean
clean:
	rm -rf src/*.o $(BUILD_DIR)/ 
