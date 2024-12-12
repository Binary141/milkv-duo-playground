all: build

build:
	$(MAKE) -C bl33
	$(MAKE) -C custom
	MONITOR_PATH=$(PWD)/custom/custom.bin LOADER_2ND_PATH=$(PWD)/bl33/bl33.bin $(MAKE) -C ../fsbl
	genimage --config genimage.cfg --tmppath /tmp/simple-boot --rootpath $(PWD)/root --inputpath $(PWD)/input

clean:
	$(MAKE) -C bl33 clean
	$(MAKE) -C custom clean
	$(MAKE) -C ../fsbl clean
	rm -rf ./images/*
	rm -rf ./root/*
	rm -rf ./input/*
