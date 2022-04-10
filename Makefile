#
#  Bootstrap Makefile
#  (C) Copyright 2022
#  John Ryland
#
# Fetches module dependencies (including build system) if not already retrieved. Avoids using
# git submodules (can be a pain to keep updated), modules should be orthogonal to the versioning
# system, so there shouldn't be a requirement to use git for either the module or parent project.
# In reality this solution currently has a dependency on the make command and using git for the
# modules (it's not smart enough yet to detect the versioning system).

MODULES=https://github.com/JohnRyland/GenericMake.git \
				https://github.com/JohnRyland/TestFramework.git

.modules/%.git:
	@echo "Fetching module: $(@:.modules/%.git=%)"
	@git clone $(filter %$(@:.modules/%=%),$(MODULES)) $@ 2> /dev/null

all: $(patsubst %.git,.modules/%.git,$(notdir $(MODULES)))     # Dependency on the modules being retrieved
	@$(MAKE) release  # We re-invoke make so the include below will work if we don't already have the modules

-include .modules/GenericMake.git/Makefile

