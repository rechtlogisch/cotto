FROM --platform=linux/amd64 gcc as build
COPY . /app
WORKDIR /app
RUN make

FROM --platform=linux/amd64 ubuntu as final
WORKDIR /app
RUN mkdir -p /app/vendor
RUN mkdir -p /app/CEZ
COPY --from=build \
    /app/cotto \
    ./
COPY --from=build \
    /app/vendor/*.so \
    ./vendor/
COPY --from=build \
    /app/CEZ/*.p12 \
    /app/CEZ/*.cer \
    ./CEZ/
