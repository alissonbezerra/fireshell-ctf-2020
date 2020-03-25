FROM ubuntu:disco

RUN adduser --home /home/ctf --disabled-password --gecos "" ctf

COPY service /home/ctf
COPY start.sh /start.sh

RUN chmod +x /start.sh && \
    chown -R root:ctf /home/ctf && \
    chmod -R 750 /home/ctf && \
    chmod 740 /home/ctf/flag

EXPOSE 1337

CMD [ "/start.sh" ]
