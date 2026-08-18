from http.server import BaseHTTPRequestHandler, HTTPServer
from urllib.parse import parse_qs

class LoginServer(BaseHTTPRequestHandler):

    def do_POST(self):
        if self.path == "/login":
            length = int(self.headers.get("Content-Length", 0))
            body = self.rfile.read(length).decode()

            data = parse_qs(body)

            username = data.get("username", [""])[0]
            password = data.get("password", [""])[0]

            print("\n--- LOGIN RECEIVED ---")
            print("Username:", username)
            print("Password:", password)

            self.send_response(200)
            self.end_headers()
            self.wfile.write(b"Login received!")

        else:
            self.send_response(404)
            self.end_headers()


server = HTTPServer(("0.0.0.0", 8080), LoginServer)

print("Server running on http://127.0.0.1:8080")
server.serve_forever()
