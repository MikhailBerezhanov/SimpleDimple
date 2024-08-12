# Our custom image with SDL support
FROM clownmik/sdl2_builder:latest
# Specify non-root user (may be overridden with '--env USER=<DESIRED USERNAME>')
ENV USER=builder
# Add non-root user with default UID:GID
RUN useradd ${USER} -m -s /bin/bash
# Run container under non-root user
USER ${USER}