.DEFAULT_GOAL := help
include build.mk
.PHONY: help build clean test basic-test smoke rebuild-generator inventory inventory-check doctor
help:
	@printf '%s\n' 'Komondor Standalone: simulator development, learning disabled by default.' 'make doctor - check toolchain and record build/environment.json' 'make build [PROFILE=release|debug|sanitize] - compile simulator' 'make test   - native unit checks and isolated runtime checks' 'make smoke  - run deterministic rule Co-TDMA example' 'make rebuild-generator - build COST generator from source (requires flex)' 'make inventory - update file provenance and shared header catalog' 'make inventory-check - check provenance freshness and implementation include rules'
build:
	$(MAKE) -C Code/main
clean:
	$(MAKE) -C Code/main clean
smoke: build
	$(PYTHON) scripts/run.py --mode tdma --traffic native --observe --nodes examples/multi_ap_native/nodes.csv --mapc examples/multi_ap_native/mapc.csv --config examples/multi_ap_native/config_models --time 0.25
# Rebuild the source-to-source compiler for another platform if needed.
rebuild-generator:
	$(MAKE) -C Code/compcpp
	cp Code/compcpp/cxx Code/COST/cxx
	chmod +x Code/COST/cxx
test: inventory-check build
	$(PYTHON) tests/check.py
	$(PYTHON) tests/basic_native_tdma.py

inventory:
	$(PYTHON) scripts/inventory.py
inventory-check:
	$(PYTHON) scripts/inventory.py --check

doctor:
	$(PYTHON) scripts/build_environment.py --record build/environment.json

basic-test: inventory-check build
	$(PYTHON) tests/basic_native_tdma.py
