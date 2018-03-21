import falcon
import constants

class RequireCSV(object):

	def process_request(self, req, resp):
		if req.method in ('POST', 'PUT'):
			if constants.MEDIA_CSV not in req.content_type:
				raise falcon.HTTPUnsupportedMediaType

class AuthMiddleware(object):

	def __init__(self, userAgentRequired):
		self._userAgentRequired = userAgentRequired

	def process_request(self, req, resp):
		authorization = req.get_header('Authorization')
		userAgent = req.get_header('User-Agent')

		if userAgent != self._userAgentRequired:
			if authorization is None: #make clients believe that Authorization header is the issue
				raise falcon.HTTPForbidden
			else:
			   raise falcon.HTTPUnauthorized
