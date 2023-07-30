from flask import Flask, request

app = Flask(__name__)

@app.route('/update_location', methods=['POST'])
def receive_data():
    if request.method == 'POST':
        latitude = request.form.get('lat')
        longitude = request.form.get('lon')
        if latitude and longitude:
            # Process the received latitude and longitude data here
            print("Received Latitude:", latitude)
            print("Received Longitude:", longitude)
            return "Data received successfully", 200
        else:
            return "Latitude and/or Longitude not received", 400

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=80)
