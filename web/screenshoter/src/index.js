const express = require('express')
const bodyParser = require('body-parser')
const { check, validationResult } = require('express-validator')
const phantom = require('phantom')
const path = require('path')
const sha1 = require('sha1')
const app = express()

app.use(bodyParser.urlencoded({ extended: true }));

app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'index.html'))
})

app.post('/', [
  check('url').isURL().custom(value => {
    return value.startsWith('http')
  })
], (req, res) => {
  const errors = validationResult(req)
  if (!errors.isEmpty()) {
    return res.send('<html><body><h1>Bad URL!</h1></body></html>')
  }

  const fileName = sha1(new Date().getTime() + Math.floor(Math.random() * 1000)) + '.png'
  const filePath = path.join(__dirname, 'tmp', fileName)

  phantom.create(["--web-security=false"]).then(ph => {
    ph.createPage().then(page => {
      page.open(req.body.url).then(() => {
        page.render(filePath).then(() => {
          res.sendFile(filePath)
          page.close().then(() => {
            ph.exit()
          })
        }).catch(() => {
          res.send('<html><body><h1>Something went wrong! I do not know why.</h1></body></html>')
        })
      }).catch(() => {
        res.send('<html><body><h1>Something went wrong! I do not know why.</h1></body></html>')
      })
    })
  })
})

app.listen(3500, () => {
  console.log('Listening on 3500')
})
