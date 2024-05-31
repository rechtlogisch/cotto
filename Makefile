.PHONY: all cotto clean

PATH_VENDOR ?= $(PWD)/vendor
ENV_FILE ?= .env
image := rechtlogisch/cotto

all: clean cotto

cotto:
	@if [ -z "$(PATH_VENDOR)" ]; then \
		echo "Error: PATH_VENDOR is not set"; \
		exit 1; \
	elif [ ! -d "$(PATH_VENDOR)" ]; then \
		echo "Error: Directory $(PATH_VENDOR) does not exist"; \
		exit 2; \
	fi

	@if [ -z "$$(find '$(PATH_VENDOR)' -name '*otto.*' -print -quit)" ]; then \
		echo "Error: No file matching *otto.{dll,dylib,so} found in $(PATH_VENDOR)"; \
		exit 3; \
	fi
	
	g++ -ldl -L"$(PATH_VENDOR)" -Wl,-rpath,"$(PATH_VENDOR)" -lotto -o cotto cotto.cpp

clean:
	rm -f cotto otto.log*

docker-build:
	docker build \
		-t $(image) \
		.

docker-cmd:
	docker run \
		-it --rm \
		--env-file=$(ENV_FILE) \
		--platform=linux/amd64 \
		-v $(PATH_DOWNLOAD):/app/download/ \
		$(image)

docker-cotto:
	docker run \
		-it --rm \
		--env-file=$(ENV_FILE) \
		--platform=linux/amd64 \
		--name=cotto \
		-v $(PATH_DOWNLOAD):/app/download/ \
		$(image) \
		./cotto $(input)
