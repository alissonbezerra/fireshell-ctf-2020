FROM pet3rmatta/phantomjs-alpine

WORKDIR /app

COPY src .

RUN echo "F#{R3ally_b4d_Ph4nT0m_!!}" > /flag && \
    npm install

EXPOSE 3500

CMD ["node", "index.js"]
