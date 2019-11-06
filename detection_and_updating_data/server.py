import http.server
import socketserver



PORT = 8080
IP="ip adress of server here"
Handler = http.server.SimpleHTTPRequestHandler

with socketserver.TCPServer((IP, PORT), Handler) as httpd:
    print("serving at port", PORT)
    httpd.serve_forever()
