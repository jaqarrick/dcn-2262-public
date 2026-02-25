FROM python:3.9-slim

RUN apt-get update && \
    apt-get install -y curl && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

RUN groupadd -g 799 nyu && \
    useradd -r -u 999 -g nyu nyu

RUN mkdir -p /app && chown -R nyu:nyu /app
WORKDIR /app

COPY --chown=nyu:nyu requirements.txt .
RUN pip install -r requirements.txt
COPY --chown=nyu:nyu . .
RUN chown -R nyu:nyu /app && chmod -R 775 /app
USER nyu

EXPOSE 53533/udp

CMD ["python", "./app.py"]