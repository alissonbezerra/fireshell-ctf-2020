#!/bin/sh

while true
do
  cd /home/ctf
  /home/ctf/firehttpd &
  sleep 15
  killall firehttpd
done