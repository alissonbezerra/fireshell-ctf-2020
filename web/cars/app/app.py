from flask import Flask, request, jsonify
from lxml import etree

app = Flask(__name__)
app.config.from_object(__name__)
app.config['SECRET_KEY'] = '3c61a3282f599eb29ab09b2ed847379f8324fc9e'

cars = [
  {'id':'1','name':'Honda Civic','year':'2019','image':'https://cars.fireshellsecurity.team/static/images/honda_civic_2019.jpeg'},
  {'id':'2','name':'Audi A3','year':'2019','image':'https://cars.fireshellsecurity.team/static/images/a3_2019.png'},
  {'id':'3','name':'Mitsubishi ASX','year':'2019','image':'https://cars.fireshellsecurity.team/static/images/asx_2019.png'},
  {'id':'4','name':'Toyota Corolla','year':'2019','image':'https://cars.fireshellsecurity.team/static/images/corolla_2019.jpeg'},
  {'id':'5','name':'Nissan Frontier','year':'2019','image':'https://cars.fireshellsecurity.team/static/images/frontier_2019.jpg'},
  {'id':'6','name':'Ford Fusion','year':'2019','image':'https://cars.fireshellsecurity.team/static/images/fusion_2019.png'},
  {'id':'7','name':'Toyota Hillux','year':'2019','image':'https://cars.fireshellsecurity.team/static/images/hillux_2019.png'},
  {'id':'8','name':'Mitsubishi L200','year':'2019','image':'https://cars.fireshellsecurity.team/static/images/l200_2019.png'},
  {'id':'9','name':'Nissan Versa','year':'2019','image':'https://cars.fireshellsecurity.team/static/images/versa_2019.png'}
]

@app.route('/')
def index():
  return 'Cars API v0.1'

@app.route('/cars', methods=['GET'])
def get_all():
  return jsonify(cars)

@app.route('/car/<int:car_id>', methods=['GET'])
def get_car(car_id):
  try:
    return jsonify(cars[car_id - 1])
  except:
    return jsonify({ 'message': 'Car not found!' }), 404

@app.route('/comment', methods=['POST'])
def post_comment():
  content_type = request.content_type

  if content_type.startswith('application/json'):
    data = request.get_json()
  elif content_type.startswith('application/xml'):
    doc = etree.fromstring(request.data)
    data = {}
    for element in doc:
      data[element.tag] = element.text
  else:
    return jsonify({ 'message': 'Bad data!' }), 400

  if 'name' not in data:
    return jsonify({ 'message': 'Missing name!' }), 400

  return jsonify({
    'message': 'Thank you {} for your comment!'.format(data['name'])
  })

if __name__ == '__main__':
  app.run(host='0.0.0.0')
