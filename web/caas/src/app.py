from flask import Flask, render_template, flash, request, send_file
from wtforms import Form, StringField, SubmitField, validators, ValidationError
import tempfile
import subprocess

app = Flask(__name__)
app.config.from_object(__name__)
app.config['SECRET_KEY'] = '4d1288af76884b985f3abb7d8aca463dc8bd4e73'

def blacklist(form, field):
  if '</flag' in field.data:
    raise ValidationError('Blacklisted code.')

class MyForm(Form):
  code = StringField('Code', validators=[validators.input_required(), blacklist])

@app.route('/', methods=['GET', 'POST'])
def index():
  form = MyForm(request.form)

  if request.method == 'POST':
    code = request.form['code']

    if form.validate():
      source_fn = tempfile.NamedTemporaryFile(prefix='caas_', suffix='.c', delete = False).name

      with open(source_fn, 'w') as source_file:
        source_file.write(code)

      dest_fn = tempfile.NamedTemporaryFile(prefix='caas_', suffix='_compiled', delete = False).name

      try:
        subprocess.check_output(['gcc', source_fn, '-o', dest_fn], stderr=subprocess.STDOUT)
        return send_file(dest_fn, as_attachment=True)
      except subprocess.CalledProcessError as e:
        flash('Sorry, we could not compile this code.')
        flash(e.output)

    else:
      flash('Bad code!')

  return render_template('index.html', form=Form)

if __name__ == '__main__':
  app.run(host='0.0.0.0')
