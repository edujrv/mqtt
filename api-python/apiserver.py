import subprocess
from flask import Flask, jsonify, request

app = Flask(__name__)

# A variable to track the process state
process = None

@app.route('/start', methods=['POST'])
def start_process():
    global process
    if process is not None and process.poll() is None:
        return jsonify({'message': 'Process is already running'}), 400

    # Execute the app.c program
    process = subprocess.Popen(['../project'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    return jsonify({'message': 'Process started successfully'})

@app.route('/stop', methods=['POST'])
def stop_process():
    global process
    if process is None or process.poll() is not None:
        return jsonify({'message': 'Process is not running'}), 400

    # Terminate the process
    process.terminate()
    process = None

    return jsonify({'message': 'Process stopped successfully'})

if __name__ == '__main__':
    app.run()
