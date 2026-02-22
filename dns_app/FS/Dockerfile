FROM python:3.9-slim

RUN apt-get update && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

RUN groupadd -g 799 nyu && \
    useradd -r -u 999 -g nyu nyu

WORKDIR /app
COPY --chown=nyu:nyu requirements.txt Makefile .
RUN pip install -r requirements.txt
USER nyu
COPY --chown=nyu:nyu . .
EXPOSE 53533/udp
CMD [ "python", "./app.py" ]