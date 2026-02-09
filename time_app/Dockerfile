FROM python:3.9-slim
# or python:3.10-slim, python:3.11-slim, etc.

RUN apt-get update && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

RUN groupadd -g 799 nyu && \
    useradd -r -u 999 -g nyu nyu

WORKDIR /app
RUN pip install Flask
USER nyu
COPY --chown=nyu:nyu . .
CMD [ "python", "./run.py" ]