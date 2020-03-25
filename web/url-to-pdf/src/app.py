from flask import Flask, render_template, flash, request
from wtforms import Form, StringField, SubmitField, validators
from flask_weasyprint import HTML, render_pdf
import hashlib

app = Flask(__name__)
app.config.from_object(__name__)
app.config['SECRET_KEY'] = '7110eda4d09e062aa5e4a390b0a572ac0d2c0220'

class MyForm(Form):
  url = StringField('URL', validators=[validators.input_required(), validators.URL(require_tld=True), validators.Regexp(r'^(http|https)://')])

@app.route('/', methods=['GET', 'POST'])
def index():
  form = MyForm(request.form)

  if request.method == 'POST':
    url = request.form['url']

    if form.validate():
      fn = hashlib.md5(url.encode('utf-8')).hexdigest() + '.pdf'
      return render_pdf(HTML(url), download_filename=fn)
    else:
      flash('Invalid URL!')

  return render_template('index.html', form=Form)

if __name__ == '__main__':
  app.run(host='0.0.0.0')
