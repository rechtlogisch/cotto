FROM --platform=linux/amd64 gcc AS build
COPY . /app
WORKDIR /app
RUN make

FROM --platform=linux/amd64 ubuntu AS final
WORKDIR /app
RUN mkdir -p /app/vendor
RUN mkdir -p /app/certificate
COPY --from=build \
    /app/cotto \
    ./
COPY --from=build \
    /app/vendor/*.so \
    ./vendor/
COPY --from=build \
    /app/certificate/*.pfx \
    ./certificate/
