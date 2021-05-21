FROM golang AS build
ARG VERSION
ARG CONTROLLER
WORKDIR /app
ADD . /app
RUN VERSION=$VERSION make bin/$CONTROLLER && mv bin/$CONTROLLER app

FROM gcr.io/distroless/base AS run
COPY --from=build /app/app /app
CMD [ "/app" ]
