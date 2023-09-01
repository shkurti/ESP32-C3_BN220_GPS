from flask_socketio import SocketIO
from flask import Flask, render_template
import mysql.connector
import threading
import time

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret!'
app.config['DEBUG'] = True

socketio = SocketIO(app, async_mode=None, logger=True, engineio_logger=True)

db_config = {
    'host': 'localhost',
    'user': 'root',
    'password': '',
    'database': 'g_data',
}

class DatabaseMonitor(threading.Thread):
    def __init__(self):
        super().__init__()
        self.stop_event = threading.Event()
        self.last_processed_id = 0
        self.last_processed_id_lock = threading.Lock()

    def run(self):
        connection = None
        while not self.stop_event.isSet():
            try:
                if connection is None or not connection.is_connected():
                    connection = mysql.connector.connect(**db_config)
                print("Querying for new data...")
                data = self.query(connection)
                if data:
                    print(f"Found new records: {data}")
                    for latitude, longitude, dt, record_id in data:
                        print(f"Emitting newlocation event: {latitude}, {longitude}")
                        socketio.emit('newlocation', {'latitude': latitude, 'longitude': longitude}, namespace='/test')
                else:
                    print("No new records found.")
                time.sleep(5)
            except mysql.connector.Error as e:
                print(f"MySQL Error: {e}")
                time.sleep(5)

    def stop(self):
        self.stop_event.set()

    def query(self, connection):
        query = f"SELECT Lng, Lat, DT, ID FROM `gps` WHERE ID > %s ORDER BY ID ASC"
        with self.last_processed_id_lock:
            with connection.cursor() as cursor:
                cursor.execute(query, (self.last_processed_id,))
                data = cursor.fetchall()
                if data:
                    self.last_processed_id = data[-1][3]
            connection.close()
            return data

@app.route('/')
def index():
    return render_template('index.html')

@socketio.on('connect', namespace='/test')
def on_connect():
    global db_monitor
    db_monitor = DatabaseMonitor()
    db_monitor.start()

@socketio.on('disconnect', namespace='/test')
def on_disconnect():
    global db_monitor
    if db_monitor:
        db_monitor.stop()
        db_monitor.join()

if __name__ == '__main__':
    db_monitor = None
    socketio.run(app)
