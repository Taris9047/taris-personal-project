.PHONY: clean All

All:
	@echo "----------Building project:[ Physics - Release ]----------"
	@cd "Physics" && $(MAKE) -f  "Physics.mk"
	@echo "----------Building project:[ BrownianWX - Release ]----------"
	@$(MAKE) -f  "BrownianWX.mk" && $(MAKE) -f  "BrownianWX.mk" PostBuild
clean:
	@echo "----------Cleaning project:[ Physics - Release ]----------"
	@cd "Physics" && $(MAKE) -f  "Physics.mk"  clean
	@echo "----------Cleaning project:[ BrownianWX - Release ]----------"
	@$(MAKE) -f  "BrownianWX.mk" clean
