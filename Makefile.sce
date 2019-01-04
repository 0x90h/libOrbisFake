ORBISDEV := 
MAKEPATH := $(ORBISDEV)/make/scesdk.mk
target := sce_lib
OutPath := lib

TargetFile := libOrbisFake
AllTarget = $(OutPath)/$(TargetFile).a

include $(MAKEPATH)


$(OutPath)/$(TargetFile).a: $(ObjectFiles)
	$(dirp)
	$(archive)

install:
	@cp $(OutPath)/$(TargetFile).a $(ORBISDEV)/lib/
	#@cp include/*.h $(ORBISDEV)/include/
	@echo "Installed!"
