from flask import request, Flask
from dotenv import load_dotenv
import socket
import os
from utils import fibonacci 

load_dotenv()

port = os.getenv('FLASK_RUN_PORT', 9090)
app = Flask(__name__)

@app.route('/register', methods=['PUT'])
def register():
    data = request.get_json()
    hostname = data.get('hostname')
    ip = data.get('ip')
    as_ip = data.get('as_ip')
    as_port = data.get('as_port')
    for param in ['hostname', 'ip', 'as_ip', 'as_port']:
        if not data.get(param):
            return {"error": f"Missing parameter: {param}"}, 400
        
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    message = f"TYPE=A\nNAME={hostname} VALUE={ip} TTL=10"
    addr = (as_ip, int(as_port)) 

    try:
        sock.sendto(message.encode(), addr)
        response, _ = sock.recvfrom(1024)
        decoded = response.decode()
        if decoded == "OK":
            return { "message": "OK"}, 201
    finally:
        sock.close()
    
@app.route('/fibonacci', methods=['GET'])
def fib():

    number = request.args.get('number')
    if not number:
        return {"error": "Missing number parameter"}, 400
    
    try: 
        number = int(number)
    except:
        return {"error": "number parameter must be an integer"}, 400

    result = fibonacci(number)
    return {
        "message": f"{result}"
    }, 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=port, debug=True)