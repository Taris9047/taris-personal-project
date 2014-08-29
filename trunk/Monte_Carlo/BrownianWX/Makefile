.PHONY: clean All

All:
	@echo "----------Building project:[ Physics - Debug ]----------"
	@cd "Physics" && $(MAKE) -f  "Physics.mk"
	@echo "----------Building project:[ BrownianWX - Debug ]----------"
	@$(MAKE) -f  "BrownianWX.mk"
clean:
	@echo "----------Cleaning project:[ Physics - Debug ]----------"
	@cd "Physics" && $(MAKE) -f  "Physics.mk"  clean
	@echo "----------Cleaning project:[ BrownianWX - Debug ]----------"
	@$(MAKE) -f  "BrownianWX.mk" clean
