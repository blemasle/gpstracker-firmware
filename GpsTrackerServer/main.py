"""
Basic HTTP server to receive and save gps positions sent by Arduino GPS Tracker
"""

import falcon

class RequireCSV:

	def process_request(self, req, resp):
		if req.method in ('POST', 'PUT'):
			if 'text/csv' not in req.content_type:
				raise falcon.HTTPUnsupportedMediaType('Only CSV is supported')

class PositionsResource:

	def on_post(self, req, resp):
		data = req.bounded_stream.read()
		resp.body = data
		resp.status = falcon.HTTP_201

positions = PositionsResource()

api = falcon.API(middleware=[
	RequireCSV()
])

api.add_route('/positions', positions)

if __name__ == "__main__":
	# Use python built-in WSGI reference implemention to run a web server
	from wsgiref.simple_server import make_server

	print('Starting web server...')
	srv = make_server('localhost', 8080, api)
	srv.serve_forever()