FROM gcc AS build
COPY . /app
WORKDIR /app
RUN make

FROM ubuntu AS final
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
