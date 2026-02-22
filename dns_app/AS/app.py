import socket
from dotenv import load_dotenv
import os
import db

load_dotenv()

db.init_db()

def main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    port = int(os.getenv('AS_PORT', 53533))
    sock.bind(('0.0.0.0', port))   

    print(f"Authoritative DNS UDP Server Listening for Connections at {port}")     
    while True:
        print("UDP Packet Received")
        data, addr = sock.recvfrom(1024)
        request = data.decode()
        parsed = {}

        for token in request.replace('\n', ' ').split(' '):
            if '=' in token:
                key, _, value = token.partition('=')
                parsed[key.strip()] = value.strip()

        print(parsed)
        name = parsed.get("NAME")
        record_type = parsed.get("TYPE")
        value = parsed.get("VALUE")
        ttl = parsed.get("TTL")

        if name is not None and record_type is not None and value is None and ttl is None:
            record = db.get_record(name, record_type)
            if record:
                record_name = record['name']
                record_type = record['type']
                record_value = record['value']
                record_ttl = record['ttl']
                response = f"TYPE={record_type}\nNAME={record_name} VALUE={record_value} TTL={record_ttl}"
                sock.sendto(response.encode(), addr)
            else:
                sock.sendto(b"Not Found", addr)
        elif record_type != "A":
            sock.sendto(b"Unsupported record type", addr)
        elif ttl is not None and value is not None:
            db.insert_record(name, record_type, value, int(ttl))
            print("DNS record saved")
            sock.sendto(b"OK", addr)

if __name__ == '__main__':
    main()