from flask import request, Flask
from dotenv import load_dotenv
import os
import socket
from utils import parse_dns_record
import requests

load_dotenv()

port = os.getenv('FLASK_RUN_PORT', 8080)
app = Flask(__name__)

@app.route('/fibonacci')
def get_fibonacci():
    params = {
        'hostname': str,
        'fs_port': str,
        'number': int,
        'as_ip': str,
        'as_port': str,
    }
    for param in params:
        if not request.args.get(param):
            return {"error": f"Missing parameter: {param}"}, 400
    
    hostname = request.args.get('hostname', type=str)
    fs_port = request.args.get('fs_port', type=str)
    number = request.args.get('number', type=int)
    as_ip = request.args.get('as_ip', type=str)
    as_port = request.args.get('as_port', type=str)


    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    message = f"TYPE=A\nNAME={hostname}"
    addr = (as_ip, int(as_port)) 

    try:
        sock.sendto(message.encode(), addr)
        response, _ = sock.recvfrom(1024)
        decoded = response.decode()
        if decoded == "Not Found":
            return { "error": "DNS Record Not Found"}, 404
        
        record = parse_dns_record(decoded)
        fs_ip = record.get("VALUE")

        if not fs_ip:
            return { "error": "could not parse IP value"}, 500

        fs_response = requests.get(f'http://{fs_ip}:{fs_port}/fibonacci?number={number}')

        data = fs_response.json()
        status = fs_response.status_code

        if status == 200:
            value = data.get("message")
            return { "message": value}, 200
        else:
            return { "error": f"FS server request failed with status {status}"}, 500
    except Exception as e:
        return { "error": str(e) }, 500
    finally:
        sock.close()


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=port, debug=True)