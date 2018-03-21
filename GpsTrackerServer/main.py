"""
Basic HTTP server to receive and save gps positions sent by Arduino GPS Tracker
"""

import api

if __name__ == "__main__":
	# Use python built-in WSGI reference implemention to run a web server
	from wsgiref.simple_server import make_server

	print('Starting web server...')
	srv = make_server('localhost', 8080, api.create())
	srv.serve_forever()
