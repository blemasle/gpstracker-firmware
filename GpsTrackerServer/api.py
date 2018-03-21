import falcon
import middlewares
import resources
import constants

#from constants import *

def create():
	api = falcon.API(media_type=constants.MEDIA_CSV, middleware=[
		middlewares.AuthMiddleware(constants.USER_AGENT),
		middlewares.RequireCSV()
	])

	api.add_route(constants.ROUTES_POSITIONS, resources.PositionsResource())

	return api