import falcon

class PositionsResource:

	def on_post(self, req, resp):
		data = req.bounded_stream.read()
		resp.body = data
		resp.status = falcon.HTTP_201

	def on_get(self, req, resp):
		raise falcon.HTTPNotImplemented
