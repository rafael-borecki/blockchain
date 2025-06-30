FROM python:3.11-slim

WORKDIR /app
COPY main.py .

RUN pip install paho-mqtt kafka-python

CMD ["python3", "-u", "main.py"]
