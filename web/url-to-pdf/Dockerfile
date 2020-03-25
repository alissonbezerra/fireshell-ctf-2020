FROM python:3.7-slim

COPY src /app
COPY flag.txt /flag

WORKDIR /app

RUN apt-get update && apt-get install -y libpango1.0-0 && \
    pip install -r requirements.txt

EXPOSE 5000

ENTRYPOINT ["python"]

CMD ["app.py"]
