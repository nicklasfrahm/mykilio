FROM golang AS build
ARG VERSION
ARG CONTROLLER
WORKDIR /app
COPY go.* .
RUN go mod download
ADD Makefile /app
ADD cmd /app/cmd
ADD pkg /app/pkg
ADD internal /app/internal
RUN VERSION=$VERSION make bin/$CONTROLLER && mv bin/$CONTROLLER app

FROM gcr.io/distroless/base AS run
COPY --from=build /app/app /
ENTRYPOINT [ "./app" ]
